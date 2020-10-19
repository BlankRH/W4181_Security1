#!/bin/bash

testdir=$HOME/test

cd ..

mkdir log

echo "CREATING Certificates"
./createCA.sh secret1 secret2 secret3 secret4 secret5 secret6

echo "VERIFYING Certs"
cd $testdir/ca/intermediate/certs

openssl verify -CAfile ca-chain.cert.pem \
    localhost.cert.pem

openssl x509 -noout -text \
    -in localhost.cert.pem

openssl verify -CAfile ca-chain.cert.pem \
    client.cert.pem

openssl x509 -noout -text \
    -in client.cert.pem

echo "VERIFYING SIGN"
cd $testdir/file
openssl dgst -sha256 \
    -verify $HOME/test/ca/intermediate/sign.pub.pem \
    -signature sign.txt.sha256 sign.txt

echo "DECRYPTING"
openssl rsautl -decrypt -inkey private/encrypt.key.pem \
     -in test.enc.txt -out test.de.txt -passin=pass:secret6
cat test.de.txt
