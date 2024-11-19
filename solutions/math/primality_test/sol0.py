# Using 'openssl prime' command to check a prime number LOL.
# Because switch between subprocess is slow, so solve the queries together.
# Input and output a hole file to accelerate IO.
import subprocess
import io, os

input = io.BytesIO(os.read(0, os.fstat(0).st_size)).readline
res = []

t = int(input())
x = [str(int(input())) for _ in range(t)]

for s in subprocess.run(["openssl", "prime", "-checks", "3", *x], 
                        capture_output=True, text=True).stdout.splitlines():
    res.append(b"Yes" if 'not' not in s else b"No")

os.write(1, b"\n".join(res) + b"\n")