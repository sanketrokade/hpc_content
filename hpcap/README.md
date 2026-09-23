# HPCAP — Hands-on Parallel Programming in C

A 21-day, example-driven course in parallel programming: **Pthreads → OpenMP → MPI**.
Every day has notes plus small, self-contained C programs you can compile and run on a laptop, a workstation, or an HPC cluster.

> Based on the original day-wise lab notes by **[CISSSCO](https://github.com/CISSSCO/hpcap)**, republished with permission and reorganized for self-study. See [CREDITS.md](CREDITS.md).

## Quick start

```bash
git clone <this-repo-url> && cd <repo>
scripts/check-env.sh          # what can this machine build/run?
make                          # build all examples (or: make pthreads | openmp | mpi)

# or work inside one day:
cd mpi/day11-point-to-point
../../scripts/compile.sh p2p_mpi.c      # picks gcc/mpicc + -fopenmp/-pthread automatically
../../scripts/run.sh ./p2p_mpi.out 4    # launches 4 MPI ranks
```

Don't have a compiler or MPI yet? → **[GETTING_STARTED.md](GETTING_STARTED.md)** (Linux, WSL, macOS, HPC clusters with modules/Spack/SLURM/PBS).

## Learning path

Work through the days in order — each page links to the previous and next day.

### Part 0 — Serial optimization
| Day | Topics | Examples |
|---|---|---|
| [Day 0](optimization/day00-compiler-optimizations/) | Serial & Compiler Optimizations | 2 |

### Part 1 — Shared memory with Pthreads
| Day | Topics | Examples |
|---|---|---|
| [Day 1](pthreads/day01-thread-basics/) | Creating Threads | 4 |
| [Day 2](pthreads/day02-race-conditions-mutex/) | Race Conditions & Mutex | 5 |
| [Day 3](pthreads/day03-reduction-condvars-barriers/) | Reduction, Condition Variables, Barriers | 15 |

### Part 2 — Shared memory with OpenMP
| Day | Topics | Examples |
|---|---|---|
| [Day 4](openmp/day04-openmp-intro/) | Introduction | 7 |
| [Day 5](openmp/day05-data-scoping/) | Data Scoping Clauses | 9 |
| [Day 6](openmp/day06-reduction-timing/) | Reduction & Timing | 6 |
| [Day 7](openmp/day07-synchronization/) | Synchronization (critical, single, master, barrier) | 12 |
| [Day 8](openmp/day08-scheduling/) | Loop Scheduling | 9 |
| [Day 9](openmp/day09-atomic-locks-tasks/) | Atomic, Locks, Sections, Tasks | 8 |

### Part 3 — Distributed memory with MPI
| Day | Topics | Examples |
|---|---|---|
| [Day 10](mpi/day10-mpi-intro/) | Introduction & Hello World | 7 |
| [Day 11](mpi/day11-point-to-point/) | Point-to-Point (Blocking/Non-blocking) | 13 |
| [Day 12](mpi/day12-bcast-tags-status/) | Broadcast, Tags, Status | 11 |
| [Day 13](mpi/day13-scatter-gather/) | Scatter & Gather | 8 |
| [Day 14](mpi/day14-reduce-allreduce/) | Reduce & Allreduce | 7 |
| [Day 15](mpi/day15-sendrecv-bsend-alltoall/) | Sendrecv, Bsend, Ssend, Alltoall | 11 |
| [Day 16](mpi/day16-derived-datatypes/) | Derived Datatypes | 13 |
| [Day 17](mpi/day17-pack-groups-communicators/) | Pack/Unpack, Probe, Groups & Communicators | 12 |
| [Day 18](mpi/day18-pi-primes-matrix/) | Pi, Prime Count, Matrix Addition | 10 |
| [Day 19](mpi/day19-matrix-mult-init-thread/) | Matrix Multiplication, MPI_Init_thread | 12 |
| [Day 20](mpi/day20-barrier-topology-performance/) | Barrier, Topologies, Performance | 2 |

Full topic list: [SYLLABUS.md](SYLLABUS.md). Further reading: [REFERENCES.md](REFERENCES.md).

## How to use each day

1. Read the day's `README.md` — code and explanation sit together; outputs are shown where recorded.
2. Compile and run each example yourself. Change thread/process counts and input sizes, then watch what changes.
3. Files named `task*.c` are exercises — attempt them before reading the solution in the notes.

## Repository layout

```
optimization/  pthreads/  openmp/  mpi/   day-wise notes + examples
scripts/       compile.sh, run.sh, check-env.sh (portable across systems)
jobs/          slurm.sbatch, pbs.sh (batch templates for clusters)
Makefile       build everything; CI builds every example on each push
```

## Known issues (good exercises)

These examples do not compile as-is and are excluded from `make`. Fixing them is good practice:

| File | Problem |
|---|---|
| `openmp/day05-data-scoping/test.c` | two `main()` functions in one file |
| `openmp/day09-atomic-locks-tasks/taskParallelism1.c` | two `main()` functions in one file |
| `mpi/day11-point-to-point/mpi_async.c` | `MPI_Isend` called with too few arguments |
| `mpi/day17-pack-groups-communicators/mpi_task_parallelism.c` | `world_group` used but never declared |

Also note: `optimization/day00-compiler-optimizations/demo2.c` intentionally fails to link at `-O0` (C99 `inline` semantics) — that's part of the day's lesson. Build it with `-O2`.

## Coming later

GPU programming tracks: CUDA, OpenACC, SYCL.

## Contributing

Found a bug or a clearer explanation? Open an issue or pull request. Keep examples small and self-contained, and make sure `make` still passes.
