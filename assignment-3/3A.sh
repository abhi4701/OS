#! /usr/bin/bash

echo -n "Enter a filename to see last modification time : "
read file
 

if [ ! -f $file ]
then
		echo "$file not a file"
else
	echo $file modified on: $(stat -c %x $file)
fi
