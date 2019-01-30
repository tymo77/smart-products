objects = SPmath.o SPmatrix.o
include_dir = ../../../include/
source_dir = ../../../source/
SPmath = $(source_dir)SPmath.cpp $(include_dir)SPmath.h 
SPmatrix = $(source_dir)SPmatrix.cpp $(include_dir)SPmatrix.h

main : $(include_dir)SPGPIO.h
	g++ -o main.out ../lab.cpp $(include_dir)SPGPIO.h

.PHONY : clean
clean :
	rm main.out $(objects)
