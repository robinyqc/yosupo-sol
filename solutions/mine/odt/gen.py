import random as rd

n = rd.randint(1, 1000000)
m = rd.randint(1, 1000000)

print(n, m)

print(*[rd.randint(0, 998244352) for _ in range(n)])

for _ in range(m):
    l = rd.randint(1, n)
    r = rd.randint(1, n)
    if l > r:
        l, r = r, l
    print(l - 1, r, rd.randint(0, 998244352))