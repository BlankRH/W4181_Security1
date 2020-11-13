#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 DIRECTORY" >&2
  exit 1
fi

if [ ! -d "$1" ]; then
  echo "$1 Does Not Exist."
  exit 1
fi

groupadd exeUser
useradd -m -d /home/mailbox/exeUser -g exeUser exeUser

chown -R exeUser:exeUser $1
setfacl -m m::r-- $1

setfacl -m m::rw $1/tmp
setfacl -m o::--- $1/tmp
setfacl -d -m o::--- $1/tmp
setfacl -d -m m::rw $1/tmp

setfacl -m m::r $1/bin
chmod u+s $1/bin/mail-in
chmod g+s $1/bin/mail-in
setfacl -m m::--x $1/bin/mail-in
setfacl -m o::--x $1/bin/mail-in
setfacl -m m::--x $1/bin/mail-out
setfacl -m o::--- $1/bin/mail-out

setfacl -R -m o::--- $1/mail

directories=`ls $1/mail`

for name in $directories
do
	setfacl -d -m u:$name:rw- $1/mail/$name
done