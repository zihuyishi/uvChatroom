root = ../../
debugDir = $(root)debug/
objects = base.o client.o

client : $(objects) 
	c++ -luv -o $(debugDir)client $(objects)

base.o : ../base.cpp $(root)include/base.h
	c++ -c ../base.cpp

client.o : client.cpp $(root)include/base.h
	c++ -c client.cpp

.PHONY :clean
clean :
	-rm $(debugDir)client $(objects) 
