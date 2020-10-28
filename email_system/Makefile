CC = g++
C_FLAGS = -Wall -g

all:
	$(CC) -o mail-in $(C_FLAGS) mail-in.cpp
	$(CC) -o mail-out $(C_FLAGS) mail-out.cpp
	cp mail-in mail-out $(DEST)

install: mail-in mail-out
	cp mail-in mail-out $(DEST)

.PHONY:clean
clean:
	rm -rf mail-in mail-out test out