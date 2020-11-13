#!/bin/bash

if [ ! -d "/home/mailbox" ]; then
  sudo ./mailbox_gen.sh
fi

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 DIRECTORY" >&2
  exit 1
fi

if [ -d "$1" ]; then
  echo "$1 Already Exists."
  exit 1
fi

mkdir "$1"

mkdir $1/bin $1/mail $1/tmp

cp mail-in mail-out $1/bin

directories=`find /home/mailbox -maxdepth 1 -mindepth 1 -type d | sed 's/\/home\/mailbox\///1'`

for name in $directories
do
   mkdir $1/mail/$name
done

mkdir $1/mail/$USER