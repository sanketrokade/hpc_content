#!/bin/bash
# Submit from the repo root:  qsub -v SRC=mpi/day14-reduce-allreduce/arrSum_reduce.c jobs/pbs.sh
# Adjust queue/walltime/modules to your cluster (check: qstat -Q, module avail).
#PBS -N hpcap
#PBS -l select=1:ncpus=4:mpiprocs=4
#PBS -l walltime=00:10:00
#PBS -j oe
##PBS -q workq                      # uncomment and set if your site requires it

set -euo pipefail
cd "$PBS_O_WORKDIR"
# module load openmpi

np=$(wc -l < "$PBS_NODEFILE")
scripts/compile.sh "$SRC"
scripts/run.sh "./${SRC%.*}.out" "$np"
