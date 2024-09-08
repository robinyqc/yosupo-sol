#!/usr/bin/env sh

# Compile the C++ program
if ! g++ -O2 -std=c++20 $1.cpp -o $1; then
    exit -1
fi

# check if pwd has a directory named "ans"
if [ ! -d "ans" ]; then
    mkdir "ans"
fi

max_time=0

# Loop through all input files matching the pattern
for i in $(ls in/$2*.in); do 
    ans=$(echo $i | sed "s/.in$/.ans/" | sed "s/^in/ans/")
    echo "from ${i} into ${ans}"
    
    # Run the program and capture the output and time usage
    time_usage=$( { /usr/bin/time -p ./$1 < $i > $ans; } 2>&1 )
    real_time=$(echo "$time_usage" | grep "real" | awk '{print $2}')

    echo "real time: $real_time"
    
    # Update the maximum time usage
    if [ $(echo "$real_time > $max_time" | bc) -eq 1 ]; then
        max_time=$real_time
    fi
done

echo "Maximum time usage: $max_time seconds"
