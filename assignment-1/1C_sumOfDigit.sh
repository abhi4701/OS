#! /usr/bin/bash

read -p 'Enter a number: ' num

cnt=0

while (( num != 0 ))
do
	res=`expr $num % 10`
        cnt=`expr $cnt + $res`
	num=`expr $num / 10`
done

echo $cnt

