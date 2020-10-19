#!/bin/bash

cd $testdir/ca
mkdir intermediate

cd intermediate
mkdir certs crl csr newcerts private
touch index.txt
touch index.txt.attr
echo 1000 > serial

echo 1000 > crlnumber

cp $testdir/intermediate-config.txt $testdir/ca/intermediate/openssl.cnf

cd $testdir/ca

openssl genrsa -aes256 -passout pass:$interpwd \
    -out intermediate/private/intermediate.key.pem 4096

openssl req -config intermediate/openssl.cnf -new -sha256 -passin=pass:$interpwd \
    -key intermediate/private/intermediate.key.pem \
    -out intermediate/csr/intermediate.csr.pem \
    -subj "/C=GB/ST=England/L=./O=Alice Ltd/OU=Alice Ltd Certificate Authority/CN=Alice Ltd Intermediate CA"

openssl ca -config openssl.cnf -extensions v3_intermediate_ca \
        -passin=pass:$rootpwd -days 3650 -notext -md sha256 -batch \
        -in intermediate/csr/intermediate.csr.pem \
        -out intermediate/certs/intermediate.cert.pem


cat intermediate/certs/intermediate.cert.pem \
      certs/ca.cert.pem > intermediate/certs/ca-chain.cert.pem
