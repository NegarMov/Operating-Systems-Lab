#!bin/bash

read -p 'Please enter x: ' x
read -p 'Please enter y: ' y
read -p "Please enter file's name: " name
count=1
while read -r line; do
	if [ $count -ge $x ] && [ $count -le $y ] ; then
		echo $line	
	fi
	count=$((count+1))
done < $name
