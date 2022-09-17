#!bin/bash

read -p "Pattern's number: " pn
read -p "n: " n

case $pn in
	'1')
		for (( i=1; i<=n; i++ )) ; do
			for (( j=0; j<i; j++ )) ; do
				echo -n $i
			done
			echo ''
		done
		;;
	'2')
		for (( i=1; i<=n; i++ )) ; do
			for (( j=0; j<n-i; j++ )) ; do
				echo -n ' '
			done
			for (( j=0; j<i; j++ )) ; do
				echo -n '* '
			done
			echo ''
		done
		for (( i=n; i>=1; i-- )) ; do
			for (( j=0; j<n-i; j++ )) ; do
				echo -n ' '
			done
			for (( j=0; j<i; j++ )) ; do
				echo -n '* '
			done
			echo ''
		done
		;;
	'3')
		for (( i=1; i<=n; i++ )) ; do
			for (( j=1; j<i; j++ )) ; do
				echo -n "| "
			done
			echo '|_'
		done
		;;
esac
