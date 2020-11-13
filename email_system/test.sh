#!/bin/bash

sudo make clean

make install

./install-unpiv.sh $1
sudo ./install-priv.sh $1

if [ ! -d "$1" ]; then
  echo "$1 Does Not Exist."
  exit 1
fi

./inputtest.sh $1
