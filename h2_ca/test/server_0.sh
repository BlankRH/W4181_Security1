#!/bin/bash

cd $HOME/test/server

openssl s_server -WWW -accept 10004 \
    -pass pass:secret3 \
    -cert certs/localhost.cert.pem \
    -CAfile certs/ca-chain.cert.pem \
    -key private/localhost.key.pem -verify_return_error \
    -Verify 2 >$HOME/4181/h2_ca/log/server.out 2>&1