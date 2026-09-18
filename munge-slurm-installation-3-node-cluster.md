# Installing MUNGE and SLURM on the 3-Node HPC Cluster

This builds directly on the **3-node OpenMPI cluster** (`master`, `node1`, `node2`) already set up. MUNGE handles authentication between nodes, and SLURM is the job scheduler that will let you submit and manage MPI jobs instead of running `mpirun` by hand.

**Assumes already done (from the previous guide):**
- `/etc/hosts` has all 3 nodes
- Passwordless SSH from master → node1, node2
- NFS share mounted at `/nfs/prog` on all nodes via autofs
- Firewall/SELinux disabled

---

## Part 0: Sync Clocks (all nodes)

MUNGE rejects requests if node clocks drift apart, so fix time sync first.

```bash
sudo dnf install -y chrony
sudo systemctl enable --now chronyd
chronyc tracking
```

---

## Part 1: Install MUNGE

### 1. Install the MUNGE packages — on all 3 nodes
```bash
sudo dnf install -y munge munge-libs munge-devel
```

### 2. Generate a shared key — on master only
```bash
sudo /usr/sbin/create-munge-key -r
```
*(MUNGE authenticates nodes using one shared secret key — every node must have the exact same file at `/etc/munge/munge.key`.)*

### 3. Copy the key to node1 and node2
```bash
sudo scp /etc/munge/munge.key sanket@node1:/tmp/munge.key
sudo scp /etc/munge/munge.key sanket@node2:/tmp/munge.key
```
Then on **each** node (node1, node2):
```bash
sudo mv /tmp/munge.key /etc/munge/munge.key
sudo chown munge:munge /etc/munge/munge.key
sudo chmod 400 /etc/munge/munge.key
```

### 4. Start and enable MUNGE — on all 3 nodes
```bash
sudo systemctl enable --now munge
sudo systemctl status munge
```

### 5. Test it
On master:
```bash
munge -n | ssh node1 unmunge
munge -n | ssh node2 unmunge
```
If you see `STATUS: Success (0)`, node1/node2 can correctly verify tokens signed by master.

---

## Part 2: Create a Matching `slurm` User (all 3 nodes)

SLURM's daemons run as a dedicated `slurm` user. The UID/GID **must match** across all nodes since there's no central directory service here.

```bash
sudo groupadd -g 981 slurm
sudo useradd -m -c "SLURM workload manager" -d /var/lib/slurm -u 981 -g slurm -s /bin/bash slurm
```

---

## Part 3: Install SLURM Build Dependencies (all 3 nodes)

```bash
sudo dnf install -y gcc gcc-c++ make python3 munge-devel readline-devel \
  pam-devel perl-ExtUtils-MakeMaker mariadb-devel numactl-devel \
  hwloc-devel lua-devel ncurses-devel rrdtool-devel wget
```

---

## Part 4: Build SLURM Once, Share It via NFS

Since `/nfs` is already shared and mounted on all three nodes, build SLURM **once on master** and install it straight into the shared folder — node1 and node2 get it automatically, no separate compiling needed.

### 6. Download the source — on master
```bash
cd /nfs/prog
wget https://download.schedmd.com/slurm/slurm-26.05.3.tar.bz2
tar -xjf slurm-26.05.3.tar.bz2
cd slurm-26.05.3
```
*(Check https://www.schedmd.com/download-slurm/ for the current version if this one's aged out.)*

### 7. Configure, build, and install into the shared path
```bash
mkdir /nfs/slurm
./configure --prefix=/nfs/slurm --sysconfdir=/nfs/slurm/etc
make -j$(nproc)
sudo make install
```

### 8. Make the shared install visible on node1 and node2
Add a new line to `/etc/auto.nfs` **on master**:
```bash
sudo vim /etc/auto.nfs
```
```
prog   -rw  master:/nfs/prog
slurm  -rw  master:/nfs/slurm
```
Reload autofs on master, node1, and node2:
```bash
sudo systemctl restart autofs
```
Confirm on node1/node2:
```bash
ls /nfs/slurm/bin
```
You should see `sinfo`, `srun`, `sbatch`, etc.

### 9. Add SLURM binaries to your PATH — on all 3 nodes
```bash
echo 'export PATH=$PATH:/nfs/slurm/bin:/nfs/slurm/sbin' >> ~/.bashrc
source ~/.bashrc
```

---

## Part 5: Configure SLURM

### 10. Create `slurm.conf` — on master
```bash
sudo mkdir -p /nfs/slurm/etc
sudo vim /nfs/slurm/etc/slurm.conf
```
```ini
ClusterName=hpccluster
SlurmctldHost=master

AuthType=auth/munge
ProctrackType=proctrack/linuxproc
ReturnToService=1

SlurmUser=slurm
SlurmctldPort=6817
SlurmdPort=6818

SlurmctldSpoolDir=/var/spool/slurmctld
SlurmdSpoolDir=/var/spool/slurmd
SlurmctldLogFile=/var/log/slurm/slurmctld.log
SlurmdLogFile=/var/log/slurm/slurmd.log

SchedulerType=sched/backfill
SelectType=select/cons_tres

NodeName=master NodeAddr=master CPUs=2 State=UNKNOWN
NodeName=node1  NodeAddr=node1  CPUs=2 State=UNKNOWN
NodeName=node2  NodeAddr=node2  CPUs=2 State=UNKNOWN

PartitionName=debug Nodes=master,node1,node2 Default=YES MaxTime=INFINITE State=UP
```
*(This config is shared automatically — no need to copy it, since `/nfs/slurm/etc` is on the NFS mount visible everywhere.)*

### 11. Create a minimal `cgroup.conf`
```bash
sudo vim /nfs/slurm/etc/cgroup.conf
```
```ini
CgroupPlugin=cgroup/v2
```

---

## Part 6: Create Local Runtime Directories

These stay **local** to each node (not shared) since each daemon writes its own spool/logs.

On master:
```bash
sudo mkdir -p /var/spool/slurmctld /var/log/slurm
sudo chown slurm:slurm /var/spool/slurmctld /var/log/slurm
```

On master, node1, and node2 (run this loop from master using the SSH access already set up):
```bash
for host in node1 node2; do
  ssh $host "sudo mkdir -p /var/spool/slurmd /var/log/slurm && sudo chown slurm:slurm /var/spool/slurmd /var/log/slurm"
done
sudo mkdir -p /var/spool/slurmd
sudo chown slurm:slurm /var/spool/slurmd
```

---

## Part 7: Start SLURM

### 12. On master — start the controller
```bash
sudo /nfs/slurm/sbin/slurmctld
```

### 13. On node1 and node2 — start the compute daemon
```bash
sudo /nfs/slurm/sbin/slurmd
```
*(For production use you'd install these as systemd services — `make install` with `--enable-slurmd-service` or copy the unit files from `etc/slurm*.service` in the source tree. Running them directly like this is fine for testing.)*

---

## Part 8: Test the Cluster

### 14. Check node status — from master
```bash
sinfo
```
Expected: all three nodes listed as `idle`.

### 15. Run a quick job across all nodes
```bash
srun -N3 hostname
```
Should print `master`, `node1`, and `node2`.

### 16. Run your existing OpenMPI program through SLURM
```bash
cd /nfs/prog
srun -N3 --ntasks=6 ./hello
```
This runs the same `hello.c` from the OpenMPI guide, but now SLURM is handling allocation instead of a manual `machinefile`.

---

## Quick Recap

| Component | Purpose |
|---|---|
| MUNGE | Authenticates communication between SLURM daemons across nodes |
| `slurm` user | Runs the SLURM daemons with matching UID/GID everywhere |
| `/nfs/slurm` | Shared SLURM install + config — build once, use everywhere |
| `slurmctld` | Master's controller daemon — schedules and tracks jobs |
| `slurmd` | Compute node daemon — runs the actual job steps |
| `slurm.conf` | Defines cluster name, nodes, CPUs, and partitions |
| `sinfo` / `srun` / `sbatch` | Check cluster state / run a job now / submit a batch job |
