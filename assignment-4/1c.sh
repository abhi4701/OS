#! /usr/bin/bash

file=$1
if [ $# -ne 1 ]
then
	echo "Invalid number of arguments"
elif [ ! -f $file ]
then
	echo "File doesnot exist"
else
	awk 'BEGIN {FS="|"}
	 {
	 print $1
	 }' Student.txt > temp1.txt
	sort temp1.txt > S31.txt
	uniq S31.txt > S3.txt
	echo "Unique names:"
	cat S3.txt
fi

