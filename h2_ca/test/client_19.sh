#!/bin/bash

name=client
rootpwd=secret1
clientpwd=secret4
interpwd=secret2
testdir=$HOME/test
log_dir=$(pwd)/../log

mv $testdir $HOME/test_tmp

cd ..

./createCA.sh secret1 secret2 secret3 secret4 secret5 secret6 >${log_dir}/out 2>&1

cd $testdir/ca

if [ ! -f  "intermediate/cert/19.${name}.cert.pem" ]; then

    openssl req -config intermediate/openssl.cnf -passin=pass:$clientpwd\
            -key $testdir/client/private/${name}.key.pem -batch \
            -new -sha256 -out intermediate/csr/19.${name}.csr.pem \
            -subj "/C=GB/ST=England/L=./O=client/OU=certificate/CN=19.client" 

    openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
            -extensions usr_cert -notext -md sha256 -batch \
            -in intermediate/csr/19.${name}.csr.pem \
            -out intermediate/certs/19.${name}.cert.pem >>${log_dir}/client.out 2>&1

   cp intermediate/certs/19.${name}.cert.pem $testdir/client/certs

fi


cd $testdir/client

echo "GET /test.txt HTTP/1.1" | \
openssl s_client -connect localhost:10004 \
    -showcerts -debug \
    -CAfile certs/ca-chain.cert.pem \
    -pass pass:$clientpwd \
    -cert certs/19.client.cert.pem \
    -key private/client.key.pem >>${log_dir}/client.out 2>&1

rm -rf ${testdir}

mv $HOME/test_tmp ${testdir}
