#! /bin/bash
file=$1
if [ $# -ne 1 ]
then
	echo "Invalid number of arguments"
elif [ ! -f $file ]
then
	echo "File doesnot exist"
else
	awk 'BEGIN { FS="|"}
	{
		print toupper($0)
	}' $file > S2.txt
	echo After replacement :
	cat S2.txt
fi

