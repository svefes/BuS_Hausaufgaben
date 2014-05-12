#!/bin/bash
IFS="
"
function verz()
{
	echo $2"Directory : "$1
	for i in `find $1 -maxdepth 1 -type f`
	do
		echo -e $2"-File : "$i
	done
	for i in `find $1 -maxdepth 1 -type d`
	do
		if [ "$i" != "$1" ]
		then
				verz $i $2"-"
		fi
	done
}
if (($#==1))
then
verz $1 ""
else
verz . ""
fi