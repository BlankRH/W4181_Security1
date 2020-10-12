#!/bin/bash

cd $HOME/test/client

openssl s_client -connect 127.0.0.1:4433 \
    -pass pass:secret4 \
    -cert certs/client.cert.pem \
    -CAfile certs/ca-chain.cert.pem \
    -state -debug \
    -key private/client.key.pem -showcerts