#! /bin/bash
file=$1
if [ $# -ne 1 ]
then
	echo "Invalid number of arguments"
elif [ ! -f $file ]
then
	echo "File doesnot exist"
else
	cat $file|head -3|tail -2
fi

