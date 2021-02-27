#! /usr/bin/bash

read -p 'Enter number of terms: ' num

a=0
b=1

if [ $num -lt 0 ]
then
	echo Not possible
else
	for ((i = 1; i <= num; i++))
	do
		echo -ne "$b "
		nxt=$(( $a + $b ))
		a=$b
		b=$nxt
	done
fi
echo 
			


