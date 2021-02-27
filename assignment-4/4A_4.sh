#! /usr/bin/bash

file=$1

if [ $# -ne 1 ]
then 
	echo Error
else
	cat $file | head -3 | tail -2
fi
