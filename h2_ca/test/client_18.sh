#!/bin/bash

name=client
clientpwd=secret4
interpwd=secret2
testdir=$HOME/test
log_dir=$(pwd)/../log

cd $testdir/ca

cp -R intermediate inter


openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
        -key $testdir/client/private/${name}.key.pem -batch \
            -x509 \
        -new -sha256 -out intermediate/certs/18.${name}.cert.pem \
        -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=18.client" >${log_dir}/client.out 2>&1

cp intermediate/certs/18.${name}.cert.pem $testdir/client/certs


cd $testdir/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -CAfile certs/ca-chain.cert.pem \
    -pass pass:$clientpwd -verify_return_error \
    -cert certs/18.client.cert.pem \
    -key private/client.key.pem >${log_dir}/client.out 2>&1

cd $testdir/ca

rm -rf intermediate

mv inter intermediate