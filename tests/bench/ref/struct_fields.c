#include "bench.h"

struct Point { int x; int y; };
struct Rect { struct Point tl; struct Point br; };

int main(int argc, char **argv) {
    int reps = bench_reps(argc, argv), rep;
    double t0 = bench_now();
    struct Rect r = {{0, 0}, {0, 0}};
    int i;
    for (rep = 0; rep < reps; rep++) {
        r.tl.x = 0; r.tl.y = 0; r.br.x = 0; r.br.y = 0;
        for (i = 0; i < 2000000; i++) {
            r.br.x = r.br.x + 1;
            r.tl.y = r.br.x - r.tl.x;
            KEEP(r.br.x);
        }
    }
    bench_report(t0, reps);
    printf("%d %d\n", r.br.x, r.tl.y);
    return 0;
}
