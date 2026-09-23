# Build every example:   make            (or: make pthreads | openmp | mpi | optimization)
# Override tools:        make CC=clang MPICC=mpiicc CFLAGS="-O3"
# Binaries are written next to their source as <name>.out (git-ignored).

CC     ?= gcc
MPICC  ?= mpicc
CFLAGS ?= -O2 -w
LDLIBS  = -lm

# Examples that currently do not compile - listed in README "Known issues".
BROKEN := openmp/day05-data-scoping/test.c \
          openmp/day09-atomic-locks-tasks/taskParallelism1.c \
          mpi/day11-point-to-point/mpi_async.c \
          mpi/day17-pack-groups-communicators/mpi_task_parallelism.c

src = $(filter-out $(BROKEN),$(wildcard $(1)/*/*.c))
OPT  := $(patsubst %.c,%.out,$(call src,optimization))
PTH  := $(patsubst %.c,%.out,$(call src,pthreads))
OMP  := $(patsubst %.c,%.out,$(call src,openmp))
MPI  := $(patsubst %.c,%.out,$(call src,mpi))

.PHONY: all optimization pthreads openmp mpi clean
all: optimization pthreads openmp mpi
optimization: $(OPT)
pthreads: $(PTH)
openmp: $(OMP)
mpi: $(MPI)

optimization/%.out: optimization/%.c ; $(CC) $(CFLAGS) $< -o $@ $(LDLIBS)
pthreads/%.out:     pthreads/%.c     ; $(CC) $(CFLAGS) -pthread $< -o $@ $(LDLIBS)
openmp/%.out:       openmp/%.c       ; $(CC) $(CFLAGS) -fopenmp $< -o $@ $(LDLIBS)
mpi/%.out:          mpi/%.c          ; $(MPICC) $(CFLAGS) -fopenmp $< -o $@ $(LDLIBS)

clean:
	find optimization pthreads openmp mpi -name '*.out' -delete
