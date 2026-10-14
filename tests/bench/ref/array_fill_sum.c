#include "bench.h"

int main(int argc, char **argv) {
    int reps = bench_reps(argc, argv), rep;
    double t0 = bench_now();
    int a[2000];
    int i, k, s = 0;
    for (rep = 0; rep < reps; rep++) {
        s = 0;
        for (k = 0; k < 1000; k++) {
            for (i = 0; i < 2000; i++) { a[i] = i; KEEP(i); }
            for (i = 0; i < 2000; i++) { s = s + a[i]; KEEP(s); }
        }
    }
    bench_report(t0, reps);
    printf("%d\n", s);
    return 0;
}
