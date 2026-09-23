# Getting Started

[Index](README.md)

You need three things: a **C compiler with OpenMP** (days 0–9), **Pthreads** (days 1–3), and an **MPI implementation** (days 10–20).
Run `scripts/check-env.sh` at any point — it tells you what is present and what is missing.

Pick the section that matches your machine.

---

## Linux — Rocky / RHEL / AlmaLinux / Fedora

```bash
sudo dnf install -y gcc make openmpi openmpi-devel environment-modules
# open a new shell so the `module` command is available, then:
module load mpi/openmpi-x86_64
```
Add the `module load` line to `~/.bashrc` to make it permanent.

## Linux — Ubuntu / Debian (and WSL2)

```bash
sudo apt update
sudo apt install -y build-essential openmpi-bin libopenmpi-dev
# MPICH instead of Open MPI also works:  sudo apt install -y mpich libmpich-dev
```

## Windows

Use **WSL2** (`wsl --install` in an admin PowerShell, then reboot), open Ubuntu, and follow the Ubuntu steps above.
Clone the repo inside the Linux filesystem (e.g. `~/`), not under `/mnt/c`, for faster builds.

## macOS (Homebrew)

Apple's default `clang` does **not** support `-fopenmp`, so use Homebrew GCC:

```bash
brew install gcc open-mpi
ls "$(brew --prefix)"/bin/gcc-*              # find the version, e.g. gcc-14
export CC=gcc-14                             # used by scripts/compile.sh and make
export OMPI_CC=gcc-14                        # makes Open MPI's mpicc use GCC too (needed for MPI+OpenMP examples)
```
Limitation: macOS does not provide `pthread_barrier_t`, so the Day 3 barrier example will not build. Everything else should.

## HPC cluster (modules / Spack / oneAPI)

Software is usually already installed; you just need to load it. Try, in this order:

```bash
module avail 2>&1 | grep -i -E 'mpi|gcc'     # environment modules / Lmod
module load gcc openmpi                       # use the exact names your site lists

spack find openmpi && spack load openmpi      # if your site uses Spack

source /opt/intel/oneapi/setvars.sh           # Intel oneAPI (path varies by site)
export MPICC=mpiicx                           # Intel MPI + icx (older installs: mpiicc)
```

**Do not run heavy jobs on the login node.** Use an interactive allocation or submit a batch job:

```bash
# SLURM
srun -N1 -n4 --pty bash                       # interactive shell on a compute node
sbatch jobs/slurm.sbatch mpi/day14-reduce-allreduce/arrSum_reduce.c

# PBS Pro / Torque
qsub -v SRC=mpi/day14-reduce-allreduce/arrSum_reduce.c jobs/pbs.sh
```
Edit the partition/queue, time limit, and `module load` lines in `jobs/` to match your site.

## No install rights at all? Use a container

```bash
docker run --rm -it -v "$PWD":/work -w /work ubuntu:24.04 \
  bash -c "apt update && apt install -y build-essential openmpi-bin libopenmpi-dev && bash"
```
`scripts/run.sh` automatically adds `--allow-run-as-root` for Open MPI inside containers.

---

## Tuning the scripts to your system

| Variable | Default | Example |
|---|---|---|
| `CC` | `gcc` | `CC=clang`, `CC=gcc-14`, `CC=icx` |
| `MPICC` | `mpicc` | `MPICC=mpiicx` |
| `CFLAGS` | `-O2 -Wall` | `CFLAGS="-O3 -march=native"` |
| `MPIRUN` | `mpirun` | `MPIRUN=srun` |
| `MPIRUN_FLAGS` | *(none)* | `MPIRUN_FLAGS="--bind-to core"` |
| `OMP_NUM_THREADS` | all cores | `OMP_NUM_THREADS=4 ./hello_omp.out` |

`scripts/run.sh` also adds `--oversubscribe` automatically when you ask Open MPI for more ranks than you have cores — handy on a laptop.

## Troubleshooting

| Symptom | Fix |
|---|---|
| `mpicc: command not found` | MPI not installed or not loaded — see your section above |
| `There are not enough slots available` | Open MPI + too many ranks: use `scripts/run.sh` (adds `--oversubscribe`) or lower N |
| `unrecognized command-line option '-fopenmp'` / `omp.h not found` | Apple clang — switch to Homebrew GCC (macOS section) |
| `mpirun has detected an attempt to run as root` | Inside a container: use `scripts/run.sh`, or add `--allow-run-as-root` |
| MPI program hangs forever | Usually a send/recv mismatch in the code (wrong rank, tag, or count) — that's often the lesson |
