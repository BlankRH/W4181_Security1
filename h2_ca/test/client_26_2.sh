#!/bin/bash
# bad_usr_cert: not include clientAuth

name=client
rootpwd=secret1
clientpwd=secret4
interpwd=secret2
testdir=$HOME/test
log_dir=$(pwd)/../log

cd $testdir/ca


cp -R intermediate inter


openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
        -key $testdir/client/private/${name}.key.pem -batch \
        -new -sha256 -out intermediate/csr/32.${name}.csr.pem \
        -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=32.client" >>${log_dir}/client.out 2>&1

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
        -extensions bad_usr_cert -notext -md sha256 -batch \
        -in intermediate/csr/32.${name}.csr.pem \
        -out intermediate/certs/32.${name}.cert.pem >>${log_dir}/client.out 2>&1

cp intermediate/certs/32.${name}.cert.pem $testdir/client/certs


cd $testdir/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -CAfile certs/ca-chain.cert.pem \
    -pass pass:$clientpwd \
    -cert certs/32.client.cert.pem \
    -key private/client.key.pem >${log_dir}/client.out 2>&1

cd $testdir/ca

rm -rf intermediate

mv inter intermediate