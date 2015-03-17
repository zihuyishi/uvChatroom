root = ../../
debugDir = $(root)debug/
objects = base.o server.o

client : $(objects)
	c++ -luv -o $(debugDir)server $(objects)

base.o : ../base.cpp $(root)include/base.h
	c++ -c ../base.cpp

server.o : server.cpp $(root)include/base.h
	c++ -c server.cpp

.PHONY :clean
clean :
	-rm $(debugDir)server $(objects)
