# Build every example:   make            (or: make pthreads | openmp | mpi | optimization)
# Override tools:        make CXX=clang++ MPICXX=mpiicpx CXXFLAGS="-O3"
# Binaries are written next to their source as <name>.out (git-ignored).

CXX    ?= g++
MPICXX ?= mpicxx
CXXFLAGS ?= -O2 -w
LDLIBS  = -lm

# Examples that currently do not compile - listed in README "Known issues".
BROKEN := openmp/day05-data-scoping/test.cpp \
          openmp/day09-atomic-locks-tasks/taskParallelism1.cpp \
          mpi/day11-point-to-point/mpi_async.cpp \
          mpi/day17-pack-groups-communicators/mpi_task_parallelism.cpp

src = $(filter-out $(BROKEN),$(wildcard $(1)/*/*.cpp))
OPT  := $(patsubst %.cpp,%.out,$(call src,optimization))
PTH  := $(patsubst %.cpp,%.out,$(call src,pthreads))
OMP  := $(patsubst %.cpp,%.out,$(call src,openmp))
MPI  := $(patsubst %.cpp,%.out,$(call src,mpi))

.PHONY: all optimization pthreads openmp mpi clean
all: optimization pthreads openmp mpi
optimization: $(OPT)
pthreads: $(PTH)
openmp: $(OMP)
mpi: $(MPI)

optimization/%.out: optimization/%.cpp ; $(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)
pthreads/%.out:     pthreads/%.cpp     ; $(CXX) $(CXXFLAGS) -pthread $< -o $@ $(LDLIBS)
openmp/%.out:       openmp/%.cpp       ; $(CXX) $(CXXFLAGS) -fopenmp $< -o $@ $(LDLIBS)
mpi/%.out:          mpi/%.cpp          ; $(MPICXX) $(CXXFLAGS) -fopenmp $< -o $@ $(LDLIBS)

clean:
	find optimization pthreads openmp mpi -name '*.out' -delete
