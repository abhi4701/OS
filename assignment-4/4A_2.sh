#! /usr/bin/bash

file=$1

if [ $# -ne 1 ]
then
	echo Error
else
	awk '{print toupper($0)}' $file 
fi

