#!/bin/bash

name=client
rootpwd=secret1
clientpwd=secret4
interpwd=secret2
testdir=$HOME/test

cd $testdir/ca


cp -R intermediate inter

openssl req -config intermediate/openssl.cnf -new -sha256 -passin=pass:$interpwd \
        -key intermediate/private/intermediate.key.pem -x509 \
        -out intermediate/certs/intermediate.cert.pem \
        -subj "/C=GB/ST=England/L=./O=Alice Ltd/OU=Alice Ltd Certificate Authority/CN=INTER" >$HOME/4181/h2_ca/log/20.client.out 2>&1


if [ ! -f  "intermediate/cert/20.${name}.cert.pem" ]; then

    openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
            -key $testdir/client/private/${name}.key.pem -batch \
            -new -sha256 -out intermediate/csr/20.${name}.csr.pem \
            -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=20.client" >$HOME/4181/h2_ca/log/20.client.out 2>&1

    openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
            -extensions usr_cert -notext -md sha256 -batch \
            -in intermediate/csr/20.${name}.csr.pem \
            -out intermediate/certs/20.${name}.cert.pem >$HOME/4181/h2_ca/log/20.client.out 2>&1

   cp intermediate/certs/20.${name}.cert.pem $testdir/client/certs

fi


cd $testdir/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -CAfile certs/ca-chain.cert.pem \
    -pass pass:$clientpwd -verify_return_error \
    -cert certs/20.client.cert.pem \
    -key private/client.key.pem >$HOME/4181/h2_ca/log/client.out 2>&1

cd $testdir/ca

mv inter intermediate