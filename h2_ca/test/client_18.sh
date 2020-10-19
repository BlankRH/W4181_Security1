#!/bin/bash

name=client
clientpwd=secret4
interpwd=secret2
testdir=$HOME/test

cd $testdir/ca

if [ ! -f  "intermediate/cert/18.${name}.csr.pem" ]; then

    openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
            -key $testdir/client/private/${name}.key.pem -batch \
             -x509 \
            -new -sha256 -out intermediate/certs/18.${name}.cert.pem \
            -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=18.client" >$HOME/4181/h2_ca/log/18.client.out 2>&1

    cp intermediate/certs/18.${name}.cert.pem $testdir/client/certs
fi

cd $testdir/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -CAfile certs/ca-chain.cert.pem \
    -pass pass:$clientpwd -verify_return_error \
    -cert certs/18.client.cert.pem \
    -key private/client.key.pem >$HOME/4181/h2_ca/log/client.out 2>&1