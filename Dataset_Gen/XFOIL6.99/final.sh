#! /bin/bash
echo "< Inside dir- $PWD"
# Compile File parsing C-program:
# "dcomp" : dataset-compiler C code executable
gcc dataset_compiler.c -o ./xdata/dcomp
truncate -s 0 failed_dcomp.txt
# Uncomment Next line to Purge the target dataset CSV file, if generating fresh samples:
#truncate -s 0 ./af_samples/dataset.csv
cd ./xdata
echo "< Inside dir- $PWD"
AoA=-2  # Set Angle of Attack value for field extraction
var=0
beginFrom=$(($AoA + 14))
ltr=""
for FILE in xdata_*.txt;
do var=$(($var + 1))
echo "File: $var";
./dcomp $FILE $beginFrom $AoA
echo "================================"
done

echo " > End of Script ---final.sh--- < " 