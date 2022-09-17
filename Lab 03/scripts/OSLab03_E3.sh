#!bin/bash

while true ; do
	read -p 'Enter your number: ' number
	if [[ $number =~ ^[0-9]+$ ]] ; then
		sum=0
		reverse=0
		while [ $number -ge 1 ] ; do
			sum=$((sum+(number%10)))
			reverse=$(((reverse*10)+(number%10)))
			number=$((number/10))
		done
		echo "Sum of digits: $sum"
		echo "Reverse: $reverse"
	else
		echo "Please enter a valid number."
	fi
done
