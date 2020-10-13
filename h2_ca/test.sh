#!/bin/bash

testdir=$HOME/test

cd $testdir/ca/intermediate/certs

openssl verify -CAfile ca-chain.cert.pem \
    127.0.0.1.cert.pem

openssl x509 -noout -text \
    -in 127.0.0.1.cert.pem

openssl verify -CAfile ca-chain.cert.pem \
    client.cert.pem

openssl x509 -noout -text \
    -in client.cert.pem


cd $testdir/file
openssl dgst -sha256 \
    -verify $HOME/test/ca/intermediate/sign.pub.pem \
    -signature sign.txt.sha256 sign.txt

#openssl rsautl -decrypt -inkey private/encrypt.key.pem \
#     -in test.enc.txt -out test.de.txt
