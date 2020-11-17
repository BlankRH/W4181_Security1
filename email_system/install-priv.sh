#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 DIRECTORY" >&2
  exit 1
fi

if [ ! -d "$1" ]; then
  echo "$1 Does Not Exist."
  exit 1
fi

random="$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 32 | head -n 1)"
groupadd inUser
useradd -s /usr/bin/false -m -d /home/inUser -g inUser inUser
echo -e "$random\n$random\n" | passwd inUser

random="$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 32 | head -n 1)"
groupadd outUser
useradd -s /usr/bin/false -m -d /home/outUser -g outUser outUser
echo -e "$random\n$random\n" | passwd outUser

chown -R root:root $1

setfacl -m u:inUser:-wx $1/tmp
setfacl -m o::--- $1/tmp

setfacl -dm o::--- $1/tmp
setfacl -dm u:inUser:-w- $1/tmp
setfacl -dm u:outUser:r-- $1/tmp

chown inUser:inUser $1/bin/mail-in

chmod u+s $1/bin/mail-in
chmod g+s $1/bin/mail-in

chown outUser:outUser $1/bin/mail-out

chmod u+s $1/bin/mail-out
chmod g+s $1/bin/mail-out

setfacl -m o::--x $1/bin/mail-in
setfacl -m u:inUser:--x $1/bin/mail-out
setfacl -m o::--- $1/bin/mail-out
setfacl -m m::--x $1/bin/mail-out

setfacl -m o::--x $1/bin

directories=`ls $1/mail`

for name in $directories
do
  setfacl -m u:${name}:rwx $1/mail/$name
  setfacl -m u:outUser:rwx $1/mail/$name
  setfacl -dm u:outUser:rw- $1/mail/$name
  setfacl -dm u:${name}:rw- $1/mail/$name
  setfacl -dm o::--- $1/mail/$name
  setfacl -dm m::rw- $1/mail/$name
done

setfacl -m u:inUser:r-x $1/mail
setfacl -m u:outUser:r-x $1/mail
setfacl -Rm o::--- $1/mail
setfacl -m o::--x $1/mail

setfacl -m o::--x $1
