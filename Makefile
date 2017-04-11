#Tell make to make one .out file for each .cpp file found in the current directory
all: $(patsubst %.cpp, %, $(wildcard *.cpp))

#Rule how to create arbitary .out files. 
#First state what is needed for them e.g. additional headers, .cpp files in an include folder...
#Then the command to create the .out file, probably you want to add further options to the g++ call.
%: %.cpp Makefile
	g++ -Wall -std=c++14 -c -g -O0 -pedantic-errors $<
	g++ -Wall -g -O0 -pedantic-errors -o $@ $@.o

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf cal date head ln ls mkdir env tail true false wc cksum pwd 
