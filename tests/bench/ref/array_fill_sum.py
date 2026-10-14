import sys
import time

reps = int(sys.argv[1]) if len(sys.argv) > 1 else 1
t0 = time.perf_counter()
for _ in range(reps):
    a = [0] * 2000
    s = 0
    k = 0
    while k < 1000:
        i = 0
        while i < 2000:
            a[i] = i
            i += 1
        i = 0
        while i < 2000:
            s = s + a[i]
            i += 1
        k += 1
print("time=%.9f" % ((time.perf_counter() - t0) / reps), file=sys.stderr)
print("%d" % s)
