import sys
import time


def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)


reps = int(sys.argv[1]) if len(sys.argv) > 1 else 1
t0 = time.perf_counter()
for _ in range(reps):
    r = fib(30)
print("time=%.9f" % ((time.perf_counter() - t0) / reps), file=sys.stderr)
print("%d" % r)
