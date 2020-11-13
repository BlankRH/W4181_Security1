#!/bin/bash

cd $1
for i in ../inputs/*
do
	echo "test ${i}"
	bin/mail-in <$i
	echo 
done