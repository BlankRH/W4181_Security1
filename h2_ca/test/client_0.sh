#!/bin/bash

cd $HOME/test/client


echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
   -CAfile certs/ca-chain.cert.pem \
    -pass pass:secret4 -verify 2 \
    -cert certs/client.cert.pem \
    -showcerts -ign_eof -verify_return_error \
    -key private/client.key.pem >$HOME/4181/h2_ca/log/client.out 2>&1 &


