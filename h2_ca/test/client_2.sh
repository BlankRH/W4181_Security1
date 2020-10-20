#!/bin/bash

log_dir=$(pwd)/../log

cd $HOME/test/client


echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
   -CAfile $HOME/test/ca/intermediate/certs/intermediate.cert.pem \
    -pass pass:secret4 -verify 2 \
    -cert certs/client.cert.pem \
    -showcerts -ign_eof -verify_return_error \
    -key private/client.key.pem >${log_dir}/client.out 2>&1 &


