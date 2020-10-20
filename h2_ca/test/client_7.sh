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
    -new -sha256 -out intermediate/csr/7.${name}.csr.pem \
    -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=7.client"

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
    -extensions usr_cert -notext -md sha256 -batch \
    -days=300 \
    -in intermediate/csr/7.${name}.csr.pem \
    -out intermediate/certs/7.${name}.cert.pem >${log_dir}/client.out 2>&1

cp intermediate/certs/7.${name}.cert.pem $testdir/client/certs
sed -i 's/.=$/a=/' $HOME/test/client/certs/7.client.cert.pem


cd $testdir/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -CAfile certs/ca-chain.cert.pem \
    -pass pass:$clientpwd \
    -cert certs/7.client.cert.pem \
    -key private/client.key.pem >${log_dir}/client.out 2>&1 &

cd $testdir/ca

rm -rf intermediate

mv inter intermediate