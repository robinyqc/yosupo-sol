for out in $(ls out/*.out); 
do 
    ans=$(echo $out | sed "s/.out$/.ans/" | sed "s/^out/ans/");
    if ! diff $out $ans > /dev/null
    then
        echo $out $ans are different
        exit -1
    fi
done