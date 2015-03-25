#define
root = ../../
debugDir = $(root)debug/
objects = base.o server.o
CC = g++
CCFLAG = -c -std=c++11  -g

#
client : $(objects)
	$(CC) -luv -o $(debugDir)server $(objects)

#object files
base.o : ../base.cpp $(root)src/base.h
	$(CC) $(CCFLAG) ../base.cpp

server.o : server.cpp $(root)src/base.h
	$(CC) $(CCFLAG)  server.cpp

.PHONY :clean
clean :
	-rm $(debugDir)server $(objects)
