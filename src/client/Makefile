root = ../../
debugDir = $(root)debug/
objects = base.o client.o
CC = g++
CCFLAG = -c -std=c++11 -g

client : $(objects) 
	$(CC) -luv -o $(debugDir)client $(objects)

base.o : ../base.cpp $(root)src/base.h
	$(CC) $(CCFLAG) ../base.cpp

client.o : client.cpp $(root)src/base.h
	$(CC) $(CCFLAG) client.cpp

.PHONY :clean
clean :
	-rm $(debugDir)client $(objects) 
