#!/usr/bin/env bash
# Report what this machine can build/run from this repo.
ok()   { printf '  [ OK ] %s\n' "$*"; }
miss() { printf '  [MISS] %s\n' "$*"; }
note() { printf '  [INFO] %s\n' "$*"; }

tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
CC="${CC:-gcc}"; MPICC="${MPICC:-mpicc}"

echo "System"
note "$(uname -sm)"
note "cores: $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null)"
grep -qi microsoft /proc/version 2>/dev/null && note "running under WSL"

echo "C compiler ($CC)"
if command -v "$CC" >/dev/null; then
    ok "$($CC --version | head -1)"
    printf '#include <pthread.h>\nint main(void){pthread_barrier_t b;(void)b;return 0;}\n' > "$tmp/p.c"
    "$CC" -pthread "$tmp/p.c" -o "$tmp/p" 2>/dev/null && ok "Pthreads (incl. pthread_barrier_t)" \
        || miss "pthread_barrier_t unavailable (normal on macOS) - day03 barrier example won't build"
    printf '#include <omp.h>\nint main(void){return omp_get_max_threads()<1;}\n' > "$tmp/o.c"
    "$CC" -fopenmp "$tmp/o.c" -o "$tmp/o" 2>/dev/null && ok "OpenMP (-fopenmp)" \
        || miss "OpenMP not supported by $CC (Apple clang? see GETTING_STARTED.md)"
else
    miss "$CC not found"
fi

echo "MPI"
if command -v "$MPICC" >/dev/null; then
    ok "$MPICC -> $($MPICC --version 2>/dev/null | head -1)"
    command -v mpirun >/dev/null && ok "mpirun: $(mpirun --version 2>&1 | head -1)" || miss "mpirun not found"
else
    miss "$MPICC not found (MPI days 10-20 need it)"
fi

echo "Cluster tooling"
command -v module >/dev/null 2>&1 || type module >/dev/null 2>&1 && note "environment modules present: try 'module avail mpi'"
command -v spack  >/dev/null && note "spack present: try 'spack find openmpi'"
command -v sbatch >/dev/null && note "SLURM detected: use jobs/slurm.sbatch"
command -v qsub   >/dev/null && note "PBS/Torque detected: use jobs/pbs.sh"
command -v bsub   >/dev/null && note "LSF detected: adapt jobs/slurm.sbatch directives to #BSUB"
[ -n "${SLURM_JOB_ID:-}" ] && note "inside SLURM job $SLURM_JOB_ID"
exit 0
