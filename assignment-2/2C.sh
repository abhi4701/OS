#! /usr/bin/bash


echo -n "Enter username: "

read user

if [ `grep -c $user /etc/passwd` -ne 0 ]
then 
	echo "User is valid"
else
	echo "User is invalid"
fi

