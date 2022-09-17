#!bin/bash

echo "${10}"

read -p 'Username: ' username
echo "Username: $username" > info.txt
read -sp 'Password: ' password
echo ''
echo "Password: $password" >> info.txt

let a=10+8
echo $a
expr 5 \* 4
expr 5 / 4
expr 11 % 2
a=$( expr 10 - 3 )
echo $a
b=$(( a + 3 ))
echo $b
((b++))
echo $b
