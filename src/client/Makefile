root = ../../
debugDir = $(root)debug/
objects = base.o client.o
CC = g++

client : $(objects) 
	$(CC) -luv -o $(debugDir)client $(objects)

base.o : ../base.cpp $(root)include/base.h
	$(CC) -c ../base.cpp

client.o : client.cpp $(root)include/base.h
	$(CC) -c client.cpp

.PHONY :clean
clean :
	-rm $(debugDir)client $(objects) 
