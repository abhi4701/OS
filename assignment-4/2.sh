#! /bin/bash
echo "Enter file name"
read file
if [ -f $file ]
then
	awk 'BEGIN {
	FS ="|" ;
       	max=-9999;
	n="";
	}
	{
	       	sum=0 
		for ( i=2;i<=NF;i++ )
		     	{ 
				sum=sum+$i
		       	}
		       	if (sum>max)
			       	{ 
					max=sum
					n=$1
			       	}
	}	
	END { 
	 print n" achieved highest marks"
	 }' $file
 else
	 echo "File doesnot exit"
 fi



		

