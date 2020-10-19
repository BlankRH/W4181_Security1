#!/bin/bash


name='localhost'


echo 1000 > $testdir/server/test.txt 
echo hello > $testdir/server/hello.txt 

cd $testdir/ca

openssl genrsa -aes256 -passout pass:$serverpwd \
      -out $testdir/server/private/${name}.key.pem 2048

openssl req -config intermediate/openssl.cnf -passin=pass:$serverpwd\
        -key $testdir/server/private/${name}.key.pem \
        -new -sha256 -out intermediate/csr/${name}.csr.pem \
        -addext "subjectAltName=IP:127.0.0.1" \
        -subj "/C=GB/ST=England/L=./O=server/OU=certificate/CN=server"

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
        -extensions server_cert -days 375 -notext -md sha256 -batch \
        -in intermediate/csr/${name}.csr.pem \
        -out intermediate/certs/${name}.cert.pem

openssl req -config intermediate/openssl.cnf -passin=pass:$serverpwd\
        -key $testdir/server/private/${name}.key.pem \
        -new -sha256 -out intermediate/csr/bad.${name}.csr.pem \
        -addext "subjectAltName=IP:127.0.0.1" \
        -subj "/C=GB/ST=England/L=./O=server/OU=certificate/CN=bad.server"

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
        -extensions bad_server_cert -notext -md sha256 -batch \
        -days 300 \
        -in intermediate/csr/bad.${name}.csr.pem \
        -out intermediate/certs/bad.${name}.cert.pem

cp intermediate/certs/${name}.cert.pem $testdir/server/certs
cp intermediate/certs/bad.${name}.cert.pem $testdir/server/certs
cp intermediate/certs/ca-chain.cert.pem $testdir/server/certs
