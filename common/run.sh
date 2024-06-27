#!/usr/bin/env sh
for i in $(ls in/$2*.in); 
do 
    ans=$(echo $i | sed "s/.in$/.ans/" | sed "s/^in/ans/");
    echo "from" ${i} "into" ${ans}
    cat $i | ./$1 > $ans; 
done