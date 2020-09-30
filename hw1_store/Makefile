#!Makefile

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

CC = gcc
LD = ld

C_FLAGS = -c -Wall -g
LD_FLAGS = 

 
all: $(C_OBJECTS)
	$(CC) -o cstore $(C_OBJECTS)
	sudo mv cstore /bin/cstore

cstore.o	: handler.h
handler.o 	: handler.h crypt.h sha256.h
crypt.o 	: crypt.h sha256.h my_aes.h 
my_aes.o 	: aes.h 

.PHONY:clean
clean:
	$(RM) -rf $(C_OBJECTS) cstore ./test *.txt non.exist ./a ./b

.PHONY: test
test:
	make all 
	./test_script.sh
