# Day 0 — Serial & Compiler Optimizations

[Index](../../README.md) · [Day 1 — Pthreads: Creating Threads](../../pthreads/day01-thread-basics/)

## Compiler Optimization Demo

### demo1.c

#### code

```c
#include<stdio.h>
int main(){
    int y = 3;
    int x = y + 6;
    printf("x = %d\n", x);
    return 0;
}
```

**Output:**

```text
x = 9
```

#### compile

```bash
file=demo1
gcc $file.c -O2 -o $file.out
#gcc -E $file.c > "$file_generated".c
```

```bash
file=demo1
#gcc $file.c -O2 -fdump-tree-all -o $file.out
gcc $file.c -O2 -fdump-tree-optimized
cat a-$file*.optimized
#gcc -E $file.c > "$file_generated".c
```

```c
;; Function main (main, funcdef_no=23, decl_uid=3375, cgraph_uid=24, symbol_order=23) (executed once)

int main ()
{
  <bb 2> [local count: 1073741824]:
  __printf_chk (1, "x = %d\n", 9);
  return 0;

}
```

#### run

```bash
file=demo1
./$file.out
```

**Output:**

```text
x = 9
```

### demo2.c

#### code

```c
#include<stdio.h>
inline int square(int x) {
    return x * x;
}
int main(){
    int x = 10;
    int y = square(x);
    printf("Square of %d : %d\n", x, y);
    return 0;
}
```

#### compile

```bash
file=demo2
gcc -O2 $file.c -o $file.out
```

```bash
file=demo2
#gcc $file.c -O2 -o $file.out
gcc $file.c -O2 -fdump-tree-optimized
cat a-$file*.optimized
```

```c

;; Function main (main, funcdef_no=24, decl_uid=3378, cgraph_uid=25, symbol_order=24) (executed once)

int main ()
{
  <bb 2> [local count: 1073741824]:
  __printf_chk (2, "Square of %d : %d\n", 10, 100);
  return 0;

}

```

#### run

```bash
file=demo2
./$file.out
```

**Output:**

```text
Square of 10 : 100
```

## Arithmetic Optimizations

### Constant folding

Simplifies constant expressions at compile time, reducing runtime calculations.

```c
int x = 3 + 10;
```

```c
int x = 13;
```

### Constant Propagation

Replaces variables with constant values if they are known at compile time, enabling further optimizations.

```c
const int x = 10;
int y = x + 3;
```

```c
int x = 10;
int y = 10 + 3;
// which eventually will become
// int y = 13;
```

### Strength Reduction

Replaces expensive operations with cheaper ones.

#### Multiplication to Bitwise

```c
x * 8;
```

```c
x << 3;
```

#### Division to Multiplication

```
x / 2;
```

```
x * 0.5;
```

### Algebraic Simplifications

Simplifies algebraic expressions to more efficient forms.

#### Removing Common Subexpressions

```c
a * (b + c) + d * (b + c);
```

```c
(a + d) * (b + c);
```

#### Simplifying Arithmetic

```c
x + 0;
y * 1;
```

```c
x;
y;
```

## Loop Optimizations

### Loop Unrolling

Increases the loop body size by replicating it multiple times, reducing the overhead of loop control.

#### Example 1

```c
for (int i = 0; i < 4; i++) {
    // Loop body
}
```

```c
//Loop body
//Loop body
//Loop body
//Loop body
```

#### Example 2

```c
int arr[N];
for (int i = 0; i < N; i++) {
    sum += arr[i];
}
```

- N iterations required

```c
int arr[N];
for (int i = 0; i < N - 1; i+=2) {
    sum += arr[i];
    sum += arr[i + 1];
}
```

- N/2 iterations required

### Loop Fusion

Merges adjacent loops with the same iteration range into a single loop.

```c
int x = 0;
int y = 0;
for (int i = 0; i < n; i++) {
    x++;
}
for (int i = 0; i < n; i++) {
    y++;
}
printf("x = %d\n", x);
printf("y = %d\n", y);
```

```c
int x = 0;
int y = 0;
for (int i = 0; i < n; i++) {
    x++;
    y++;
}
printf("x = %d\n", x);
printf("y = %d\n", y);
```

### Loop Interchange

Swaps inner and outer loops to improve cache performance.

```c
for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
        // Loop body
    }
}
```

```c
for (int j = 0; j < m; j++) {
    for (int i = 0; i < n; i++) {
        // Loop body
    }
}
```

### Loop Invariant Code Motion

Moves code that does not change within the loop outside of the loop.

```c
int y = 0;
for (int i = 0; i < n; i++) {
    int x = 5; // Invariant code
    y+= x;
}
printf("y = %d\n", y);
```

```c
int x = 5;
for (int i = 0; i < n; i++) {
    y+= x;
}
printf("y = %d\n", y);
```

## Function Inlining

Replaces a function call with the function's code to avoid the overhead of a call and return.

```c
inline int square(int x) {
    return x * x;
}

int y = square(5);
```

```c
int y = 5 * 5;
```

## Dead Code Elimination

Removes code that will never be executed or whose results are never used.

```c
int main(){
    if(0){
    // Dead code
    }
    int x = 10;
    printf("x = %d\n",x);
    return x;
    x = 20; // Dead code
}
```

```c
int main(){
    int x = 10;
    printf("x = %d\n",x);
    return x;
}
```

**Output:**

```text
x = 10
```

---
[Index](../../README.md) · [Day 1 — Pthreads: Creating Threads](../../pthreads/day01-thread-basics/)
