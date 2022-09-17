#!bin/bash

case $2 in
	'+')
		echo -n "$1 + $3 = "
		echo  "$1 + $3" | bc
		;;
	'-')
		echo -n "$1 - $3 = "
		echo  "$1 - $3" | bc
		;;
	'x')
		echo -n "$1 x $3 = "
		echo  "$1 * $3" | bc -l
		;;
	'/')
		echo -n "$1 / $3 = "
		echo  "result = $1 / $3; scale = 2; result / 1" | bc -l
		;;
	*)
		echo "Invalid Operation!"
		;;
esac
