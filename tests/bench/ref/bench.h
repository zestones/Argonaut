/* Shared by the C references of tests/bench. Each reference runs its body `reps` times
 * (argv[1], default 1), prints the result once and reports the body time per repetition on
 * stderr as "time=<seconds>", which run_bench.py reads. */
#ifndef BENCH_H
#define BENCH_H
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Optimisation barrier: x stays live, so the loop that computes it cannot be folded away. */
#define KEEP(x) __asm__ volatile("" : "+r"(x))
#define NOINLINE __attribute__((noinline))

static int bench_reps(int argc, char **argv) { return argc > 1 ? (int)strtol(argv[1], NULL, 10) : 1; }

static double bench_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static void bench_report(double t0, int reps) { fprintf(stderr, "time=%.9f\n", (bench_now() - t0) / reps); }
#endif
