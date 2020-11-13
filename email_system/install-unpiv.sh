#!/bin/bash

if [ ! -d "$DIR" ]; then
  echo "${DIR} Already Exists."
  exit 1
fi

mkdir "$1"

mkdir $1/bin $1/mail $1/tmp

cp mail-in mail-out $1/bin

input=$(ls /home/mailbox | grep `^d')
while IFS='\n' read -r line
do
    name=$(echo ${line} | tr -d "\r")
    mkdir $1/mail/$name
done < "$input"

