import sys
import time

reps = int(sys.argv[1]) if len(sys.argv) > 1 else 1
t0 = time.perf_counter()
for _ in range(reps):
    s = 0
    i = 0
    while i < 5000000:
        s = s + 1
        i += 1
print("time=%.9f" % ((time.perf_counter() - t0) / reps), file=sys.stderr)
print("%d" % s)
