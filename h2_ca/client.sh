#!/bin/bash

name='client'

cd $testdir/client

cd $testdir/ca

openssl genrsa -aes256 -passout pass:$clientpwd \
      -out $testdir/client/private/${name}.key.pem 2048
chmod 400 $testdir/client/private/${name}.key.pem

openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
        -key $testdir/client/private/${name}.key.pem \
        -new -sha256 -out intermediate/csr/${name}.csr.pem \
        -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=client"

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
        -extensions usr_cert -days 375 -notext -md sha256 \
        -in intermediate/csr/${name}.csr.pem \
        -out intermediate/certs/${name}.cert.pem
chmod 444 intermediate/certs/${name}.cert.pem

cp intermediate/certs/ca-chain.cert.pem $testdir/client/certs
cp intermediate/certs/${name}.cert.pem $testdir/client/certs