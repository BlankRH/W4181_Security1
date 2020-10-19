#!/bin/bash

testdir=$HOME/test
export testdir

rootpwd=$1
export rootpwd

interpwd=$2
export interpwd

serverpwd=$3
export serverpwd

clientpwd=$4
export clientpwd

signpwd=$5
export signpwd

encryptpwd=$6
export encryptpwd

rm -rf $testdir

mkdir $testdir
mkdir $testdir/file
mkdir $testdir/file/private
mkdir $testdir/file/certs
mkdir $testdir/server
mkdir $testdir/server/private
mkdir $testdir/server/certs
mkdir $testdir/client
mkdir $testdir/client/private
mkdir $testdir/client/certs

cp root-config.txt $testdir/root-config.txt
cp intermediate-config.txt $testdir/intermediate-config.txt

echo root certificate
./rootPair.sh

echo intermediate certificate
./interPair.sh

echo server
./server.sh 

echo client
./client.sh

echo sign
./sign.sh # signpwd interpwd

echo encrypt
./encrypt.sh