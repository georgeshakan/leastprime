# file: $HOME/ece_4822/homework/hw_01/program/Makefile
#
# Note that this make file uses "-g" to compile and link "debug".
#

# define a target for the application
#
all: program.exe

# define a target to link the application
# -lm is the math library
# -O2 is the optimization level
program.exe: program.o 
	g++ -fopenmp -O2 -std=c++17  program.o -o program.exe

# define targets to compile the source code
#
program.o: program.cpp program.hpp Makefile
	g++ -fopenmp -O2 -std=c++17 -c    program.cpp -o program.o

# define a debug target
#
debug:
	g++ -g -o program.exe program.cpp

# define a target to clean the directory
#
clean:
	rm -f program.o program.exe

#
# end of file
