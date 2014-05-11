#!/bin/bash
count=0
function verz()
{	
	str=""
	for (( var=1 ; var<=$2; var++  )) 
	do
		str=$str"\t" 
	done
	echo "$str Directory : $1"
	for i in `find $1 -maxdepth 1 -type f`
	do
		echo -e $str\t "File : "$i
	done
	for i in `find $1 -maxdepth 1 -type d`
	do
		if [ "$i" != "$1" ]
		then
				verz $i $count+1
		fi
	done
}
if (($#==1))
then
verz $1 $count
else
verz . $count
fi
