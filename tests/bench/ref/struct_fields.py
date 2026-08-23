import sys
import time


class Point:
    __slots__ = ("x", "y")

    def __init__(self):
        self.x = 0
        self.y = 0


class Rect:
    __slots__ = ("tl", "br")

    def __init__(self):
        self.tl = Point()
        self.br = Point()


reps = int(sys.argv[1]) if len(sys.argv) > 1 else 1
t0 = time.perf_counter()
for _ in range(reps):
    r = Rect()
    r.tl.x = 0
    r.tl.y = 0
    r.br.x = 0
    r.br.y = 0
    i = 0
    while i < 2000000:
        r.br.x = r.br.x + 1
        r.tl.y = r.br.x - r.tl.x
        i += 1
print("time=%.9f" % ((time.perf_counter() - t0) / reps), file=sys.stderr)
print("%d %d" % (r.br.x, r.tl.y))
