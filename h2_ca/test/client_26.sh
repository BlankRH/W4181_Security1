#!/bin/bash

name=client
clientpwd=secret4
interpwd=secret2
testdir=$HOME/test
signpwd=secret5

cd $testdir/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -pass pass:$signpwd -verify_return_error \
    -cert $testdir/ca/intermediate/certs/sign.cert.pem \
    -key $testdir/file/private/sign.key.pem >$HOME/4181/h2_ca/log/client.out 2>&1
