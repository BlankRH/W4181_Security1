#!/bin/bash

cd $HOME/test/server

openssl s_server -WWW \
    -pass pass:secret3 \
    -cert certs/127.0.0.1.cert.pem \
    -CAfile certs/ca-chain.cert.pem \
    -key private/127.0.0.1.key.pem 
    -Verify 1