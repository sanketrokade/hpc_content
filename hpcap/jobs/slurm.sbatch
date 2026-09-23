#!/bin/bash
# Submit from the repo root:  sbatch jobs/slurm.sbatch mpi/day14-reduce-allreduce/arrSum_reduce.c
# Adjust partition/time/modules to your cluster (check: sinfo, module avail).
#SBATCH --job-name=hpcap
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --time=00:10:00
#SBATCH --output=%x-%j.log
##SBATCH --partition=standard      # uncomment and set if your site requires it

set -euo pipefail
# module load openmpi               # or: module load intel-mpi / spack load openmpi

src="${1:?pass the .c file to build and run}"
scripts/compile.sh "$src"
# mpirun works inside an allocation for Open MPI/MPICH/Intel MPI; use MPIRUN=srun if your site prefers it
scripts/run.sh "./${src%.*}.out" "$SLURM_NTASKS"
