#! /usr/bin/bash

num=$1

if [ $# -ne 1 ]
then 
	echo Invalid number of arguments
else	

result=0
for (( i = 1; i <= num; i++ ))
do
	result=`expr $result + $i`
done

echo Sum of first $num natural is $result

fi
