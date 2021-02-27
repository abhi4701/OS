#! /usr/bin/bash

read -p 'Enter the Size of array: ' n

echo "Enter The array: "
read -a num

for ((i=0; i<n; i++))
do
	for ((j=0; j<n-i-1; j++))
	do
		if [ ${num[j]} -gt ${num[$((j+1))]} ]
		then
			tmp=${num[j]}
			num[$j]=${num[$((j+1))]}
			num[$((j+1))]=$tmp	
		fi
	done
done


echo "Sorted Array is: "
echo ${num[*]}
