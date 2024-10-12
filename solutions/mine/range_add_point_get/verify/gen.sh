#!/usr/bin/bash

python gen.py 1 20 20 100 > in/small1.in
python gen.py 2 20 20 100 > in/small2.in
python gen.py 3 20 20 100 > in/small3.in
python gen.py 4 20 20 100 > in/small4.in

python gen.py 5 2000 2000 10000 > in/medium1.in
python gen.py 6 2000 2000 10000 > in/medium2.in
python gen.py 7 2000 2000 10000 > in/medium3.in
python gen.py 8 2000 2000 10000 > in/medium4.in

python gen.py 9 300000 300000 1000000000 > in/large1.in
python gen.py 10 300000 300000 1000000000 > in/large2.in