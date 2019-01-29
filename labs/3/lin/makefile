objects = SPmath.o SPmatrix.o
include_dir = ../../../include/
source_dir = ../../../source/
SPmath = $(source_dir)SPmath.cpp $(include_dir)SPmath.h
SPmatrix = $(source_dir)SPmatrix.cpp $(include_dir)SPmatrix.h

main : SPmath.o SPmatrix.o
	g++ -o main.out ../lab.cpp $(objects)

SPmath.o : $(SPmath)
	g++ -c $(SPmath)

SPmatrix.o : $(SPmatrix) $(SPmath)
	g++ -c $(SPmatrix)


.PHONY : clean
clean :
	rm main.out $(objects)
