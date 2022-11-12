#! /bin/bash
echo "< Inside dir- $PWD"
# Entering the Dir containing airfoil files:
cd ./coord_seligFmt
echo "< Inside dir- $PWD >"
# Compiling C-program: tidy_reader.c
gcc tidy_reader.c -o afreader

# Logging Files:
success="exit_success.txt"
failed="exit_failed.txt"
# Emptying File contents before directory iteration
> $success
> $failed
echo "Contents of $success:"
cat $success
echo "Contents of $failed:"
cat $failed

#Counter variable:
var=1 

#Iterating over all .dat files in the directory:
for FILE in *.dat; 
do var=$(($var + 1))
echo "File: $var";
./afreader $FILE
PREV_FILE=$( tail -n 1 $success )
if [[ "$PREV_FILE" != "$FILE" ]]; then
    printf "$FILE\n" >> $failed
fi
#echo -n "#";
done

echo ">> Script End <<"



