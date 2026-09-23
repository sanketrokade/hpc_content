#!/usr/bin/env bash
# Compile one example. Picks the compiler and flags from what the source uses.
#   usage: scripts/compile.sh file.c        -> produces file.out
# Override:  CC=clang MPICC=mpiicc CFLAGS="-O3" scripts/compile.sh file.c
set -euo pipefail

src="${1:?usage: $0 file.c}"
[ -f "$src" ] || { echo "error: $src not found" >&2; exit 1; }
out="${src%.*}.out"

CC="${CC:-gcc}"
MPICC="${MPICC:-mpicc}"
CFLAGS="${CFLAGS:--O2 -Wall}"
flags=()

if grep -qE '#include *[<"]mpi\.h' "$src"; then
    compiler="$MPICC"
    command -v "$compiler" >/dev/null || { echo "error: $compiler not found. See GETTING_STARTED.md (load an MPI module or install OpenMPI/MPICH)." >&2; exit 1; }
else
    compiler="$CC"
fi
grep -qE '#include *<omp\.h>|#pragma +omp' "$src" && flags+=(-fopenmp)
grep -qE '#include *<pthread\.h>' "$src" && flags+=(-pthread)

cmd=("$compiler" $CFLAGS ${flags[@]+"${flags[@]}"} "$src" -o "$out" -lm)
echo "+ ${cmd[*]}"
"${cmd[@]}"
echo "built: $out"
