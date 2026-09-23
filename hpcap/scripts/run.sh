#!/usr/bin/env bash
# Launch an MPI example with N processes, adapting to the MPI in use.
#   usage: scripts/run.sh ./file.out N [program args...]
# Override the launcher:  MPIRUN=srun scripts/run.sh ./file.out 4
# Extra launcher flags:   MPIRUN_FLAGS="--bind-to core" scripts/run.sh ./file.out 4
set -euo pipefail

bin="${1:?usage: $0 ./file.out N [args...]}"
np="${2:?usage: $0 ./file.out N [args...]}"
shift 2
[ -x "$bin" ] || { echo "error: $bin not found or not executable (compile it first)" >&2; exit 1; }

launcher="${MPIRUN:-mpirun}"
command -v "$launcher" >/dev/null || { echo "error: $launcher not found. See GETTING_STARTED.md." >&2; exit 1; }

extra=()
if [ "$(basename "$launcher")" = "srun" ]; then
    np_flag=(-n "$np")
else
    np_flag=(-np "$np")
    # Open MPI refuses more ranks than cores and refuses root unless told otherwise.
    # (MPICH / Intel MPI allow both by default.)
    if "$launcher" --version 2>&1 | grep -q "Open MPI"; then
        cores=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)
        [ "$np" -gt "$cores" ] && extra+=(--oversubscribe)
        [ "$(id -u)" -eq 0 ] && extra+=(--allow-run-as-root)
    fi
fi

cmd=("$launcher" ${MPIRUN_FLAGS:-} ${extra[@]+"${extra[@]}"} "${np_flag[@]}" "$bin" "$@")
echo "+ ${cmd[*]}"
"${cmd[@]}"
