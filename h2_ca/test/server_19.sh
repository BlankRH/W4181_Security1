#!/bin/bash
log_dir=$(pwd)/../log
cd $HOME/test/server

openssl s_server -WWW -accept 10004 \
    -pass pass:secret3 \
    -cert certs/localhost.cert.pem \
    -key private/localhost.key.pem -verify_return_error \
    -Verify 2 >${log_dir}/server.out 2>&1