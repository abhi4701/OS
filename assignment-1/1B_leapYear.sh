#! /usr/bin/bash

read -p "Enter Year: " year

if [ `expr $year % 400` -eq 0 ] 
then
	echo $year is leap year

elif [ `expr $year % 100` -eq 0 ] 
then
	echo $year is not leap year

elif [ `expr $year % 4 ` -eq 0 ] 
then
	echo $year is leap year
else
	echo $year is not leap year
fi


