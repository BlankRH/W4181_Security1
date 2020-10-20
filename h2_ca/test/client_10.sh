#!/bin/bash

log_dir=$(pwd)/../log

cd $HOME/test/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -CAfile certs/ca-chain.cert.pem \
    -pass pass:secret4 -verify_return_error \
    -cert certs/expired.client.cert.pem \
    -key private/client.key.pem >${log_dir}/client.out 2>&1 &