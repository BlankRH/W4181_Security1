#!/bin/bash

for i in ${input[@]}
do
	random="$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 32 | head -n 1)"
	groupadd $i
	useradd -s /usr/bin/false -m -d /home/mailbox/$i  -g $i $i
	echo -e "$random\n$random\n" | passwd $i

done