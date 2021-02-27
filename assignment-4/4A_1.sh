#! /usr/bin/bash

file=$1

if [ $# -ne 1 ]
then 
	echo Error
else
	if [ -f $file ]
	then
		sort -t "|" -rk 2rn $file > sorted.txt | head -4
	else
		echo "$file doesn't exist"
	fi
fi
