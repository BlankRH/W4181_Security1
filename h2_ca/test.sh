#!/bin/bash

testdir=$HOME/test

openssl verify -CAfile $testdir/ca/intermediate/certs/ca-chain.cert.pem \
    intermediate/certs/127.0.0.1.cert.pem


cd $HOME/test/file
openssl dgst -sha256 \
    -verify $HOME/test/ca/intermediate/sign.pub.pem \
    -signature sign.txt.sha256 sign.txt

openssl rsautl -decrypt -inkey private/encrypt.key.pem \
     -in test.enc.txt -out test.de.txt
