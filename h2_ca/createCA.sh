#!/bin/bash

testdir=$HOME/test
export testdir

rootpwd=$1
export rootpwd

interpwd=$2
export interpwd

rm -rf $testdir

mkdir $testdir


cp root-config.txt $testdir/root-config.txt
cp intermediate-config.txt $testdir/intermediate-config.txt

./rootPair.sh

./interPair.sh

./server.sh

./client.sh