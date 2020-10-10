#!/bin/bash

passwd='secret'
name='bob@example.com'

cd $testdir/ca

openssl genrsa -aes256 -passout pass:$passwd \
      -out intermediate/private/${name}.key.pem 2048

chmod 400 intermediate/private/${name}.key.pem

openssl req -config intermediate/openssl.cnf -passin=pass:$passwd\
        -key intermediate/private/${name}.key.pem \
        -new -sha256 -out intermediate/csr/${name}.csr.pem

openssl ca -config intermediate/openssl.cnf -passin=pass:$rootpwd \
        -extensions server_cert -days 375 -notext -md sha256 \
        -in intermediate/csr/${name}.csr.pem \
        -out intermediate/certs/${name}.cert.pem

chmod 444 intermediate/certs/${name}.cert.pem

openssl x509 -noout -text \
      -in intermediate/certs/${name}.cert.pem

openssl verify -CAfile intermediate/certs/ca-chain.cert.pem \
      intermediate/certs/${name}.cert.pem