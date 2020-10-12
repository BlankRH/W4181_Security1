passwd='secret'
name='client'

mkdir $testdir/client
cd $testdir/client
mkdir certs private

cd $testdir/ca

openssl genrsa -aes256 -passout pass:$passwd \
      -out $testdir/client/private/${name}.key.pem 2048
chmod 400 $testdir/client/private/${name}.key.pem

openssl req -config intermediate/openssl.cnf -passin=pass:$passwd\
        -key $testdir/client/private/${name}.key.pem \
        -new -sha256 -out intermediate/csr/${name}.csr.pem

openssl ca -config intermediate/openssl.cnf -passin=pass:$rootpwd \
        -extensions usr_cert -days 375 -notext -md sha256 \
        -in intermediate/csr/${name}.csr.pem \
        -out intermediate/certs/${name}.cert.pem

cp intermediate/certs/ca-chain.cert.pem $testdir/client/certs