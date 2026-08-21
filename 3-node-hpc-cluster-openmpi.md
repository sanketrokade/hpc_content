# Building a 3-Node HPC Cluster with OpenMPI (CentOS 9)

This guide sets up a small HPC (High Performance Computing) cluster: one **master** node and two **worker** nodes (**node1**, **node2**), connected so they can run parallel programs together using **OpenMPI** and share files using **NFS**.

---

## Part 1: Prepare the Base Virtual Machine

### 1. Install CentOS 9
Install CentOS 9 on one virtual machine. This VM will later be cloned to create node1 and node2 — so get it fully configured first.

### 2. Log in as root and update/install packages
```bash
su -
```
```bash
yum update -y
yum install nfs-utils openmpi openmpi-devel autofs epel-release -y
```
**What this does:**
- `nfs-utils`, `autofs` → let machines share a folder over the network (NFS).
- `openmpi`, `openmpi-devel` → the library that lets a program run across multiple machines at once.
- `epel-release` → adds an extra software repository.

### 3. Give your user sudo (admin) rights without a password
```bash
visudo
```
Add this line (replace `username` with your actual login username):
```
username    ALL=(ALL)      NOPASSWD:ALL
```
Save and exit.

### 4. Disable the firewall
```bash
systemctl stop firewalld
systemctl disable firewalld
```
*(The cluster nodes need to talk to each other freely, so the firewall is turned off for simplicity.)*

### 5. Disable SELinux
```bash
vi /etc/selinux/config
```
Change:
```
SELINUX=enforcing
```
to:
```
SELINUX=disabled
```
Save and exit, then leave the root session:
```bash
exit
```

### 6. Add OpenMPI to your PATH (as your normal user)
```bash
cd
vim .bashrc
```
Add at the end of the file:
```
PATH=$PATH:/usr/lib64/openmpi/bin
#LD_LIBRARY_PATH=/usr/lib64/openmpi/lib
```
Save the file. *(This lets you run MPI commands like `mpicc` and `mpirun` from anywhere.)*

### 7. Shut down the VM
```bash
sudo shutdown now
```

---

## Part 2: Clone to Create the 3 Nodes

1. **Clone this VM twice** — name the clones `node1` and `node2`. The original becomes your `master`.
2. **Start all three VMs.**
3. Log in as your **normal user** (not root) on each one.
4. Set each machine's hostname:

   On master:
   ```bash
   sudo hostnamectl set-hostname master
   ```
   On node1:
   ```bash
   sudo hostnamectl set-hostname node1
   ```
   On node2:
   ```bash
   sudo hostnamectl set-hostname node2
   ```

---

## Part 3: Network Setup

### 8. Find each VM's IP address
```bash
ip a
```
Note the IP address of master, node1, and node2.

### 9. Update `/etc/hosts` on **all three** nodes
```bash
sudo vim /etc/hosts
```
Add (using your actual IPs — don't delete existing lines):
```
192.168.230.128   master
192.168.230.129   node1
192.168.230.130   node2
```
*(This lets the nodes refer to each other by name instead of IP.)*

### 10. Set up passwordless SSH from master to the nodes
On the **master**, as your normal user:
```bash
cd
ssh-keygen          # press Enter at every prompt
ssh-copy-id -i .ssh/id_rsa.pub your-username@node1
ssh-copy-id -i .ssh/id_rsa.pub your-username@node2
```
Type `yes` and enter your password when prompted, for each node.
*(This lets master run commands on node1/node2 without asking for a password each time — required for MPI to launch processes remotely.)*

---

## Part 4: Shared Storage (NFS) — On the Master

### 11. Create a shared folder
```bash
sudo mkdir -p /nfs/prog
sudo chown your-username /nfs/prog
```

### 12. Share it via NFS
```bash
sudo vim /etc/exports
```
Add:
```
/nfs     *(rw,sync,no_root_squash)
```

### 13. Start the NFS and RPC services
```bash
sudo systemctl start nfs-server
sudo systemctl status nfs-server
sudo systemctl enable nfs-server

sudo systemctl start rpcbind
sudo systemctl status rpcbind
sudo systemctl enable rpcbind
```

### 14. Verify the share
```bash
showmount -e localhost
```
Expected output:
```
/nfs   *
```

---

## Part 5: Mount the Shared Folder — On node1 and node2

Log in as your normal user (not root) and repeat these steps on **both** node1 and node2.

### 15. Configure autofs
```bash
sudo vim /etc/auto.master
```
Add:
```
/nfs      /etc/auto.nfs
```

### 16. Create the local mount point
```bash
sudo mkdir /nfs
```

### 17. Define what to mount
```bash
sudo vim /etc/auto.nfs
```
Add:
```
prog -rw  master:/nfs/prog
```

### 18. Start autofs
```bash
sudo systemctl start autofs
sudo systemctl enable autofs
sudo systemctl status autofs
```
*(This automatically mounts master's `/nfs/prog` folder onto node1 and node2 whenever it's accessed — so all three machines see the same files.)*

---

## Part 6: Write and Run an MPI Program

### 19. On the master — create the test program
```bash
cd /nfs/prog
vim hello.c
```
```c
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);              /* Starts MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); /* Get current process id */
    MPI_Comm_size(MPI_COMM_WORLD, &size); /* Get number of processes */

    printf("Hello World from %d of %d !\n", rank, size);

    MPI_Finalize();                       /* End of MPI */

    return 0;
}
```

### 20. Confirm the file is visible on the other nodes
On node1 and node2:
```bash
ls /nfs/prog
```
You should see `hello.c` — proof that NFS sharing is working.

### 21. Compile it (on the master)
```bash
mpicc -o hello hello.c
```

### 22. Run it
```bash
mpirun ./hello
```

### 23. Run it across all three nodes using a machinefile
Create a file named `machinefile`:
```
master slots=2
node1  slots=2
node2  slots=2
```
Then run:
```bash
mpirun --machinefile machinefile ./hello
```
*(This tells MPI to launch 2 processes on each of the three machines — 6 processes total, running in parallel across the cluster.)*

---

## Part 7: A Second Test Program (Optional)

This version also prints which physical machine each process ran on:
```c
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    int rank, len, size;
    char name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &len);

    printf("Hello World!!! from process %d on %s out of %d processes!!!\n", rank, name, size);

    MPI_Finalize();

    return 0;
}
```
Compile and run it the same way as `hello.c` (steps 21–23) to confirm processes are actually distributed across master, node1, and node2.

---

## Quick Recap

| Component | Purpose |
|---|---|
| OpenMPI | Runs one program as many parallel processes across machines |
| NFS | Shares one folder (`/nfs/prog`) so all nodes see the same source/binary files |
| autofs | Auto-mounts the shared NFS folder on the worker nodes |
| Passwordless SSH | Lets the master launch processes on node1/node2 without manual login |
| `/etc/hosts` | Lets nodes address each other by name (master, node1, node2) |
| machinefile | Tells `mpirun` how many processes to start on each node |
