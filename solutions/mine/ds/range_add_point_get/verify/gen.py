import random as rd
import sys

arg = sys.argv
arg = arg[1:]

s, n, m, v = map(int, arg)

rd.seed(s)

print(n, m)

for _ in range(m):
    op = rd.randint(0, 1)
    if op == 0:
        l = rd.randint(1, n)
        r = rd.randint(1, n)
        if l > r:
            l, r = r, l
        print(op, l - 1, r, rd.randint(-v, v))
    else:
        x = rd.randint(0, n - 1)
        print(op, x)