# HPL on a Single Laptop (WSL2)

First hands-on run of HPL (High-Performance Linpack), the benchmark used to rank the TOP500 supercomputers — built from source and run on a laptop under WSL2.

**Result: 140.46 GFLOPS, residual check PASSED** (N = 28,416, NB = 192, P×Q = 2×5, 108.91 s)

---

## 1. What HPL does (simple version)

- Generates a random dense system of **N linear equations with N unknowns** (Ax = b) in double precision.
- Solves it using LU factorization, split across MPI processes.
- Counts the operations (≈ ⅔N³) and divides by run time → **GFLOPS**.
- Verifies the answer with a scaled residual; it must be **< 16** for the score to count.

Key terms:

| Term | Meaning |
|---|---|
| **N** | Problem size (matrix is N × N). Bigger = better score, limited by RAM |
| **NB** | Block size — the matrix is cut into NB × NB pieces and dealt to processes |
| **P × Q** | Process grid (rows × columns). Keep near-square, P ≤ Q |
| **Rpeak** | Theoretical max = cores × clock (GHz) × FLOPs per cycle |
| **Rmax** | What HPL actually achieves |
| **Efficiency** | Rmax ÷ Rpeak |

Why the matrix is dealt out "block-cyclically" (like cards) instead of one big chunk per process: LU finishes the matrix from the top-left corner inward, so a contiguous split leaves most processes idle near the end. Cyclic dealing keeps all of them busy.

---

## 2. Environment

| Item | Value |
|---|---|
| CPU | Intel Core i5-1235U (hybrid: 2 P-cores + 8 E-cores = 10 cores / 12 threads, AVX2 + FMA, no AVX-512) |
| Host RAM | 16 GB |
| OS | Windows 11 + WSL2 (WSL 2.7.7), Ubuntu |
| RAM visible to WSL | 8,171,184,128 bytes (~7.6 GiB — WSL2 defaults to 50% of host RAM) |
| MPI | OpenMPI (Ubuntu package) |
| BLAS | OpenBLAS (Ubuntu package) |
| HPL | 2.3 (Dec 2018, netlib) |

---

## 3. Sizing N

Rule: use ~80% of available RAM; each matrix element is 8 bytes (double); round N down to a multiple of NB.

```
0.8 × 8,171,184,128 B ÷ 8 B = 817,118,413 elements
√817,118,413          ≈ 28,585
28,585 ÷ 192          = 148.9 → 148 × 192 = 28,416
```

**N = 28,416, NB = 192**

---

## 4. Build

```bash
cd ~        # build in the Linux filesystem, NOT /mnt/c (very slow)
sudo apt update && sudo apt install -y build-essential openmpi-bin libopenmpi-dev libopenblas-dev
wget https://netlib.org/benchmark/hpl/hpl-2.3.tar.gz
tar xzf hpl-2.3.tar.gz && cd hpl-2.3
cp setup/Make.Linux_PII_CBLAS Make.linux
vim Make.linux
```

Changes in `Make.linux`:

```make
ARCH         = linux
TOPdir       = $(HOME)/hpl-2.3
MPdir        =
MPinc        =
MPlib        =
LAinc        =
LAlib        = -lopenblas
HPL_OPTS     = -DHPL_CALL_CBLAS
CC           = mpicc
LINKER       = mpicc
```

```bash
make arch=linux
ls bin/linux/        # xhpl  HPL.dat
```

Sanity test (default HPL.dat, tiny problems):

```bash
cd bin/linux
export OPENBLAS_NUM_THREADS=1
mpirun -np 4 ./xhpl          # 864/864 tests PASSED
```

---

## 5. HPL.dat used

```
HPLinpack benchmark input file
Innovative Computing Laboratory, University of Tennessee
HPL.out      output file name (if any)
6            device out (6=stdout,7=stderr,file)
1            # of problems sizes (N)
28416        Ns
1            # of NBs
192          NBs
0            PMAP process mapping (0=Row-,1=Column-major)
1            # of process grids (P x Q)
2            Ps
5            Qs
16.0         threshold
1            # of panel fact
2            PFACTs (0=left, 1=Crout, 2=Right)
1            # of recursive stopping criterium
4            NBMINs (>= 1)
1            # of panels in recursion
2            NDIVs
1            # of recursive panel fact.
1            RFACTs (0=left, 1=Crout, 2=Right)
1            # of broadcast
0            BCASTs (0=1rg,1=1rM,2=2rg,3=2rM,4=Lng,5=LnM)
1            # of lookahead depth
0            DEPTHs (>=0)
2            SWAP (0=bin-exch,1=long,2=mix)
64           swapping threshold
0            L1 in (0=transposed,1=no-transposed) form
0            U  in (0=transposed,1=no-transposed) form
1            Equilibration (0=no,1=yes)
8            memory alignment in double (> 0)
```

Note: the PFACT / NBMIN / RFACT counts multiply. The default file (3 × 2 × 3) would have run **18 full solves** at this N.

---

## 6. Run

```bash
export OPENBLAS_NUM_THREADS=1
mpirun --use-hwthread-cpus -np 10 ./xhpl | tee run1.txt
```

---

## 7. Problems hit and fixes

| # | Symptom | Cause | Fix |
|---|---|---|---|
| 1 | `ld: cannot find /usr/local/mpi/lib/libmpich.a` | Template's `MPdir/MPinc/MPlib` still pointed at an MPICH path | Blank all three — `mpicc` already adds OpenMPI paths (`mpicc --showme` to see them) |
| 2 | `prte-rmaps-base:alloc-error` (help text missing) | OpenMPI counts physical cores as slots; WSL presents fewer "cores" than 10, so 10 ranks didn't fit | `mpirun --use-hwthread-cpus` |
| 3 | (avoided) Default HPL.dat would run 18 solves | PFACT × NBMIN × RFACT combinations | Set one value each |
| 4 | (avoided) Oversubscription | OpenBLAS spawns its own threads in every MPI rank | `export OPENBLAS_NUM_THREADS=1` |

---

## 8. Result

```
T/V                N    NB     P     Q               Time                 Gflops
--------------------------------------------------------------------------------
WR00C2R4       28416   192     2     5             108.91             1.4046e+02
||Ax-b||_oo/(eps*(||A||_oo*||x||_oo+||b||_oo)*N)=   1.50358292e-03 ...... PASSED
```

Check: ⅔ × 28,416³ ≈ 1.53 × 10¹³ operations ÷ 108.91 s ≈ **140 GFLOPS** ✓

---

## 9. Analysis

**Rpeak is ambiguous on this CPU** because the clock varies (1.3 GHz base → 4.4 GHz max turbo) and the cores are of two kinds.

| Assumption | Calculation | Rpeak | Efficiency |
|---|---|---|---|
| Base clocks | 2 P × 1.3 × 16 + 8 E × 0.9 × 8 | ~99 GFLOPS | 142% (impossible → turbo was active) |
| Max turbo | 2 P × 4.4 × 16 + 8 E × 3.3 × 8 | ~352 GFLOPS | **~40%** |

- **Fact:** P-cores (AVX2, 2 FMA units) = 16 DP FLOPs/cycle.
- **Inference:** E-cores ≈ 8 DP FLOPs/cycle (narrower vector units) — not independently verified.
- **Observed:** Task Manager showed ~2.36 GHz at ~83% utilization during the run — far below 4.4 GHz turbo. A 15 W laptop chip cannot sustain full turbo on all cores under heavy AVX load (power/thermal limit).
- **Inference:** Hybrid cores also hurt HPL — every rank gets equal work, so the slower E-cores set the pace and the P-cores wait.

**Takeaway:** the ~40% is the laptop's power limit, not a broken setup. Real clusters use server CPUs with predictable sustained clocks, which makes Rpeak meaningful.

---

## 10. Next experiments

- [ ] Sweep NB (128, 192, 256) — keep everything else fixed
- [ ] Compare grids: 2×5 vs 1×10
- [ ] Try `BCAST = 1` and `DEPTH = 1`
- [ ] 12 ranks vs 10 ranks (does hyper-threading help or hurt?)
- [ ] Log clock speed during the run (`turbostat` / Task Manager)
- [ ] Multi-node run on the PXE-booted lab cluster
