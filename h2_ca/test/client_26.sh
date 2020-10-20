#!/bin/bash


name=client
clientpwd=secret4
interpwd=secret2
testdir=$HOME/test
signpwd=secret5
log_dir=$(pwd)/../log

cd $testdir/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -CAfile certs/ca-chain.cert.pem \
    -pass pass:$signpwd -verify_return_error \
    -cert $testdir/ca/intermediate/certs/sign.cert.pem \
    -key $testdir/file/private/sign.key.pem >${log_dir}/client.out 2>&1
