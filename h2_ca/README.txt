First run:

Create certificates and run basic validation on certificates:
Also include validation on signature and file decryption:

./test.sh


Then run:

Test valid client&server certificate:

Run normal server: ./server_0.sh
Run normal client: ./client_0.sh


Test validation error on server side (error code in server.out)

1. Run Normal Server: ./server_0.sh
2. Run client_x.sh in another terminal; should see corresponding error code x


Test validation error on client side (error code in client.out)

Error 19: 
    1. Run Server: ./server_19.sh
    2. Run client: ./client_0.sh

Error 2:
    1. Run Server: ./server_0.sh
    2. Run client: ./client_2.sh


