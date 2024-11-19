# Using 'factor' command to factorize a number LOL.
import subprocess

def factorize(x: int) -> list:
    y = subprocess.run(["factor", str(x)], capture_output=True, text=True).stdout.split()[1:]
    return list(map(int, y))

t = int(input())

for _ in range(t):
    f = factorize(int(input()))
    print(len(f), *f)