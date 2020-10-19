#!/bin/bash

mkdir $testdir/ca
cp $testdir/root-config.txt $testdir/ca/openssl.cnf

cd $testdir/ca
mkdir certs crl newcerts private

touch index.txt
touch index.txt.attr

echo 1000 > serial

openssl genrsa -aes256 -passout pass:$rootpwd -out private/ca.key.pem 4096

openssl req -passin pass:$rootpwd -config openssl.cnf -key private/ca.key.pem \
    -new -x509 -days 7300 -sha256 -extensions v3_ca \
    -out certs/ca.cert.pem \
    -subj "/C=GB/ST=England/L=./O=Alice Ltd/OU=Alice Ltd Certificate Authority/CN=Alice Ltd Root CA"


#openssl x509 -noout -text -in certs/ca.cert.pem