#!/bin/bash

testdir=$HOME/test
signpwd=$1
interpwd=$2

echo hello > $testdir/file/sign.txt

cd $testdir/ca

openssl genrsa -aes256 -passout pass:$signpwd \
    -out $testdir/file/private/sign.key.pem 4096
chmod 400 $testdir/file/private/sign.key.pem

openssl req -config intermediate/openssl.cnf -new -sha256 -passin=pass:$signpwd \
    -key $testdir/file/private/sign.key.pem \
    -out intermediate/csr/sign.csr.pem \
    -subj "/C=GB/ST=England/L=./O=sign/OU=certificate/CN=signature"

openssl ca -config intermediate/openssl.cnf -extensions sign_cert \
        -passin=pass:$interpwd -days 365 -notext -md sha256 \
        -in intermediate/csr/sign.csr.pem \
        -out intermediate/certs/sign.cert.pem

openssl x509 -in intermediate/certs/sign.cert.pem -pubkey -out intermediate/sign.pub.pem

cp intermediate/certs/sign.cert.pem $testdir/file/certs/sign.cert.pem

openssl dgst -sha256 -sign $testdir/file/private/sign.key.pem -passin=pass:$signpwd \
    -out $testdir/file/sign.txt.sha256 $testdir/file/sign.txt

