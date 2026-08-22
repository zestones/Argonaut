#include "bench.h"

static NOINLINE int fib(int n) {
    if (n <= 1) { return n; }
    return fib(n - 1) + fib(n - 2);
}

int main(int argc, char **argv) {
    int reps = bench_reps(argc, argv), rep;
    double t0 = bench_now();
    int r = 0;
    for (rep = 0; rep < reps; rep++) { r = fib(30); KEEP(r); }
    bench_report(t0, reps);
    printf("%d\n", r);
    return 0;
}
