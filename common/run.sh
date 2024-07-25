#!/usr/bin/env sh
if ! g++ -O2 -std=c++20 $1.cpp -o $1
then
    exit -1
fi
for i in $(ls in/$2*.in); 
do 
    ans=$(echo $i | sed "s/.in$/.ans/" | sed "s/^in/ans/");
    echo "from" ${i} "into" ${ans}
    cat $i | ./$1 > $ans; 
done