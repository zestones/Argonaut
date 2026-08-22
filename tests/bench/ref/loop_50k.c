#include "bench.h"

int main(int argc, char **argv) {
    int reps = bench_reps(argc, argv), rep;
    double t0 = bench_now();
    int i, s = 0;
    for (rep = 0; rep < reps; rep++) {
        s = 0;
        for (i = 0; i < 50000; i++) { s = s + i; KEEP(s); }
    }
    bench_report(t0, reps);
    printf("%d\n", s);
    return 0;
}
