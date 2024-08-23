#!/usr/bin/env sh
for ans in $(ls ans/$1*.ans); 
do 
    out=$(echo $ans | sed "s/.ans$/.out/" | sed "s/^ans/out/")
    if ! diff $ans $out > /dev/null -w;
    then
        echo $ans $out are different
        # exit -1
    fi
done