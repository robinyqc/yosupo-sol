for i in $(ls in/*.in); 
do 
    ans=$(echo $i | sed "s/.in$/.ans/" | sed "s/^in/ans/");
    # echo ${i} ${ans}
    cat $i | ./sol > $ans; 
done