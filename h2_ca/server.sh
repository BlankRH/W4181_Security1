#!/bin/bash


name='127.0.0.1'

echo 1000 > $testdir/server/test.txt 
echo hello > $testdir/server/hello.txt 

cd $testdir/ca

openssl genrsa -aes256 -passout pass:$serverpwd \
      -out $testdir/server/private/${name}.key.pem 2048
chmod 400 $testdir/server/private/${name}.key.pem

openssl req -config intermediate/openssl.cnf -passin=pass:$serverpwd\
        -key $testdir/server/private/${name}.key.pem \
        -new -sha256 -out intermediate/csr/${name}.csr.pem \
        -subj "/C=GB/ST=England/L=./O=server/OU=certificate/CN="${name}

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
        -extensions server_cert -days 375 -notext -md sha256 \
        -in intermediate/csr/${name}.csr.pem \
        -out intermediate/certs/${name}.cert.pem

chmod 444 intermediate/certs/${name}.cert.pem

#openssl x509 -noout -text \
#      -in intermediate/certs/${name}.cert.pem

#openssl verify -CAfile intermediate/certs/ca-chain.cert.pem \
#      intermediate/certs/${name}.cert.pem

cp intermediate/certs/${name}.cert.pem $testdir/server/certs
cp intermediate/certs/ca-chain.cert.pem $testdir/server/certs
