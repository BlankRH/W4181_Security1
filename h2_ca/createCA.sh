#!/bin/bash

testdir=$HOME/test
export testdir

rootpwd='secretpassword'
export rootpwd

interpwd='secretpassword'
export interpwd

#rm -rf $testdir

#mkdir $testdir
#cp root-config.txt $testdir/root-config.txt
#cp intermediate-config.txt $testdir/intermediate-config.txt

#./rootPair.sh

#./interPair.sh

# ./sign.sh

./revoke.sh