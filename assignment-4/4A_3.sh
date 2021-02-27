#! /usr/bin/bash

file=$1
if [ $# -ne 1 ]

then
	echo "ERROR"
elif [ ! -f $file ]
then
	echo "$File doesnot exist"
else
	awk 'BEGIN {FS="|"}
	 {
	 print $1
	 }' $file > temp1.txt
	sort temp1.txt > S31.txt
	uniq S31.txt > unique.txt
fi

