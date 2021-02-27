#! /usr/bin/bash

file=$1

if [ -f $file ]
then
	awk 'BEGIN{
		FS = "|";
		max = -99999;
		name = "";
		}{
			sum=0
			for(i = 2; i <= NF; i++){
				sum += $i;
			}
		if(sum > max){
			max = sum
			name = $1
		}	
		}
		END{
			print "Maximum marks is obtained by: " name
		}' $file > name.txt
else
	echo "$file doesn't exist"
fi
