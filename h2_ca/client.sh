#!/bin/bash

name='client'

cd $testdir/client

cd $testdir/ca

openssl genrsa -aes256 -passout pass:$clientpwd \
      -out $testdir/client/private/${name}.key.pem 2048

openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
        -key $testdir/client/private/${name}.key.pem \
        -new -sha256 -out intermediate/csr/${name}.csr.pem \
        -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=client"

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
        -extensions usr_cert -days 375 -notext -md sha256 -batch \
        -in intermediate/csr/${name}.csr.pem \
        -out intermediate/certs/${name}.cert.pem

openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
        -key $testdir/client/private/${name}.key.pem \
        -new -sha256 -out intermediate/csr/expired.${name}.csr.pem \
        -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=expired.client"

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
        -extensions usr_cert -notext -md sha256 -batch \
        -startdate 19000101000000Z -enddate 20000101000000Z \
        -in intermediate/csr/expired.${name}.csr.pem \
        -out intermediate/certs/expired.${name}.cert.pem

openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
        -key $testdir/client/private/${name}.key.pem \
        -new -sha256 -out intermediate/csr/9.${name}.csr.pem \
        -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=9.client"

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
        -extensions usr_cert -notext -md sha256 -batch \
        -startdate 20300101000000Z -enddate 20400101000000Z \
        -in intermediate/csr/9.${name}.csr.pem \
        -out intermediate/certs/9.${name}.cert.pem


cp intermediate/certs/ca-chain.cert.pem $testdir/client/certs
cp intermediate/certs/expired.${name}.cert.pem $testdir/client/certs
cp intermediate/certs/9.${name}.cert.pem $testdir/client/certs
cp intermediate/certs/${name}.cert.pem $testdir/client/certs
cp intermediate/certs/23.${name}.cert.pem $testdir/client/certs