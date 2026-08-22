import sys
import time


def inc(x):
    return x + 1


reps = int(sys.argv[1]) if len(sys.argv) > 1 else 1
t0 = time.perf_counter()
for _ in range(reps):
    acc = 0
    i = 0
    while i < 2000000:
        acc = inc(acc)
        i += 1
print("time=%.9f" % ((time.perf_counter() - t0) / reps), file=sys.stderr)
print("%d" % acc)
