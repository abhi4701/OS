#! /usr/bin/bash


echo "Choose An option: "

echo "1. check file permission: "
echo "2. Check number of files and folder"
echo "3. Check number of user connected"
echo "4. Exit"

read opt


case $opt in
	1)
		echo -n "Enter file name: "
		read file

		if [ -a $file ]
		then
			echo "Permission of file $file is: `stat -c "%A" $file | cut -b 2-10`."
		else
			echo "File Doesn't exist"
		fi
	;;
	2)
		echo "Files: `ls -p | grep -v / | wc -l`."
		echo "Directories: `ls -p | grep / | wc -l`."
	;;

	3)
		echo "Users connected is/are: `users | wc -l`."
	;;

	4)
		exit 1
	;;
esac


