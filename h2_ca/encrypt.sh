#!/bin/bash

testdir=$HOME/test
#encryptpwd=$1
#interpwd=$2

cd $testdir/ca
echo atest > $testdir/file/test.txt

filename=test.txt

openssl genrsa -aes256 -passout pass:$encryptpwd \
    -out $testdir/file/private/encrypt.key.pem 4096

openssl req -config intermediate/openssl.cnf -new -sha256 -passin=pass:$encryptpwd \
    -key $testdir/file/private/encrypt.key.pem \
    -out intermediate/csr/encrypt.csr.pem \
    -subj "/C=GB/ST=England/L=./O=encrypt/OU=certificate/CN=encryption"

openssl ca -config intermediate/openssl.cnf -extensions encrypt_cert \
        -passin=pass:$interpwd -days 30 -notext -md sha256 -batch \
        -in intermediate/csr/encrypt.csr.pem \
        -out intermediate/certs/encrypt.cert.pem

openssl x509 -in intermediate/certs/encrypt.cert.pem -pubkey -out intermediate/encrypt.pub.pem

cp intermediate/certs/encrypt.cert.pem $testdir/file/certs/encrypt.cert.pem

openssl rsautl -encrypt -pubin -inkey intermediate/encrypt.pub.pem -in $testdir/file/test.txt -out $testdir/file/test.enc.txt
