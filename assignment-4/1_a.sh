#! /bin/bash
file=$1
if [ $# -ne 1 ]
then
	echo "Invalid number of arguments"
elif [ ! -f $file ]
then
	echo "File doesnot exist"
else
	sort -t "|" -rnk 2 $file > ans1.txt
	cat ans1.txt
fi

		
