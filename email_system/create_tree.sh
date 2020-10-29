#!/bin/bash

./g.sh

rm -rf "$1"

mkdir "$1"

mkdir $1/bin $1/mail $1/tmp
cp -R inputs $1/

input="mailbox_list.txt"
while IFS='\n' read -r line
do
    name=$(echo ${line} | tr -d "\r")
    mkdir $1/mail/$name
done < "$input"
