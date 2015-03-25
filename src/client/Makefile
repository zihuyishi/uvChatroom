root = ../../
debugDir = $(root)debug/
objects = base.o client.o
SRC = $(root)src/
CC = g++
CCFLAG = -c -std=c++11 -g

client : $(objects) 
	$(CC) -luv -o $(debugDir)client $(objects)

base.o : ../base.cpp $(SRC)base.h
	$(CC) $(CCFLAG) ../base.cpp

client.o : client.cpp $(SRC)base.h $(SRC)user.h
	$(CC) $(CCFLAG) client.cpp

.PHONY :clean
clean :
	-rm $(debugDir)client $(objects) 
