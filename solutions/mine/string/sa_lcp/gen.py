import random as rd
import string

n = 1000

print(''.join(rd.choices(string.ascii_lowercase, k = n)))

q = 1000
print(q)

for _ in range(q):
    x = rd.randint(0, n - 1)
    y = rd.randint(0, n - 1)
    print(x, y)