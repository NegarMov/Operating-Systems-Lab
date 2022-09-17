#!bin/bash

if [[ $1 =~ ^-?[0-9]+$ ]] && [[ $2 =~ ^-?[0-9]+$ ]] ; then
	echo "Sum: $(($1+$2))";
	if [ $2 -gt $1 ] ; then
		echo "Max: $2"
	else
		echo "Max: $1"
	fi
else
	echo "Invalid Input: Please enter two integer numbers as argument.";
fi
