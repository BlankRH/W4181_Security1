#!/bin/bash

./create_tree.sh $1

make install DEST=$1/bin

cd $1
for i in inputs/*
do
	echo "test ${i}"
	bin/mail-in <$i
	echo 
done