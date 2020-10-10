#!/bin/bash

cd $testdir/ca

url='www.example.com'

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
      -gencrl -out intermediate/crl/intermediate.crl.pem

openssl crl -in intermediate/crl/intermediate.crl.pem -noout -text

openssl ca -config intermediate/openssl.cnf -passin=pass:$interpwd \
      -revoke intermediate/certs/${url}.cert.pem