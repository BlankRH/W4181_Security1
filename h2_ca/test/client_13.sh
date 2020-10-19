#!/bin/bash

name=client
clientpwd=secret4
interpwd=secret2
testdir=$HOME/test

cd $testdir/ca

if [ ! -f  "intermediate/csr/13.${name}.csr.pem" ]; then

    openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
            -key $testdir/client/private/${name}.key.pem -batch \
            -new -sha256 -out intermediate/csr/13.${name}.csr.pem \
            -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=13.client" >$HOME/4181/h2_ca/log/13.client.out 2>&1

    openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
            -extensions usr_cert -notext -md sha256 -batch \
            -startdate 20000101000000 -enddate 20300101000000 \
            -in intermediate/csr/13.${name}.csr.pem \
            -out intermediate/certs/13.${name}.cert.pem 

    cp intermediate/certs/13.${name}.cert.pem $testdir/client/certs
fi

cd $testdir/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -CAfile certs/ca-chain.cert.pem \
    -pass pass:$clientpwd -verify_return_error \
    -cert certs/13.client.cert.pem \
    -key private/client.key.pem >$HOME/4181/h2_ca/log/client.out 2>&1