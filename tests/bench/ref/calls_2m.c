#include "bench.h"

static NOINLINE int inc(int x) { return x + 1; }

int main(int argc, char **argv) {
    int reps = bench_reps(argc, argv), rep;
    double t0 = bench_now();
    int i, acc = 0;
    for (rep = 0; rep < reps; rep++) {
        acc = 0;
        for (i = 0; i < 2000000; i++) { acc = inc(acc); KEEP(acc); }
    }
    bench_report(t0, reps);
    printf("%d\n", acc);
    return 0;
}
