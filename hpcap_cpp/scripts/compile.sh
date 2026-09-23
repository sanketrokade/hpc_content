#!/usr/bin/env bash
# Compile one example. Picks the compiler and flags from what the source uses.
#   usage: scripts/compile.sh file.cpp      -> produces file.out
# Override:  CXX=clang++ MPICXX=mpiicpx CXXFLAGS="-O3" scripts/compile.sh file.cpp
set -euo pipefail

src="${1:?usage: $0 file.cpp}"
[ -f "$src" ] || { echo "error: $src not found" >&2; exit 1; }
out="${src%.*}.out"

CXX="${CXX:-g++}"
MPICXX="${MPICXX:-mpicxx}"
CXXFLAGS="${CXXFLAGS:--O2 -Wall}"
flags=()

if grep -qE '#include *[<"]mpi\.h' "$src"; then
    compiler="$MPICXX"
    command -v "$compiler" >/dev/null || { echo "error: $compiler not found. See GETTING_STARTED.md (load an MPI module or install OpenMPI/MPICH)." >&2; exit 1; }
else
    compiler="$CXX"
fi
grep -qE '#include *<omp\.h>|#pragma +omp' "$src" && flags+=(-fopenmp)
grep -qE '#include *<pthread\.h>' "$src" && flags+=(-pthread)

cmd=("$compiler" $CXXFLAGS ${flags[@]+"${flags[@]}"} "$src" -o "$out" -lm)
echo "+ ${cmd[*]}"
"${cmd[@]}"
echo "built: $out"
