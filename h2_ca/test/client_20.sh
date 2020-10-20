#!/bin/bash

name=client
rootpwd=secret1
clientpwd=secret4
interpwd=secret2
testdir=$HOME/test
log_dir=$(pwd)/../log

cd $testdir/ca

cp -R intermediate inter

openssl req -config intermediate/openssl.cnf -new -sha256 -passin=pass:$interpwd \
        -key intermediate/private/intermediate.key.pem -x509 \
        -out intermediate/certs/intermediate.cert.pem \
        -subj "/C=GB/ST=England/L=./O=Alice Ltd/OU=Alice Ltd Certificate Authority/CN=INTER" >${log_dir}/client.out 2>&1


openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
        -key $testdir/client/private/${name}.key.pem -batch \
        -new -sha256 -out intermediate/csr/20.${name}.csr.pem \
        -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=20.client" 

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
        -extensions usr_cert -notext -md sha256 -batch \
        -in intermediate/csr/20.${name}.csr.pem \
        -out intermediate/certs/20.${name}.cert.pem >>${log_dir}/client.out 2>&1

cp intermediate/certs/20.${name}.cert.pem $testdir/client/certs



cd $testdir/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -CAfile certs/ca-chain.cert.pem \
    -pass pass:$clientpwd \
    -cert certs/20.client.cert.pem \
    -key private/client.key.pem >${log_dir}/client.out 2>&1

cd $testdir/ca

rm -rf intermediate

mv inter intermediate
