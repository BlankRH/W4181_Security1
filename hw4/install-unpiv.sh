#!/bin/bash


if [ "$#" -ne 1 ]; then
  echo "Usage: $0 DIRECTORY" >&2
  exit 1
fi

mkdir "$1"

mkdir $1/bin $1/mail $1/tmp

directories=`find /home/mailbox -maxdepth 1 -mindepth 1 -type d | sed 's/\/home\/mailbox\///1'`

for name in $directories
do
   mkdir $1/mail/$name
done

mkdir $1/mail/$USER