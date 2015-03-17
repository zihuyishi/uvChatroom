#define
root = ../../
debugDir = $(root)debug/
objects = base.o server.o
CC = g++

#
client : $(objects)
	$(CC) -luv -o $(debugDir)server $(objects)

#object files
base.o : ../base.cpp $(root)include/base.h
	$(CC) -c ../base.cpp

server.o : server.cpp $(root)include/base.h
	$(CC) -c server.cpp

.PHONY :clean
clean :
	-rm $(debugDir)server $(objects)
