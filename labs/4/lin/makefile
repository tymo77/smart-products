objects = mySPGPIO.o
include_dir = ../../../include/
source_dir = ../../../source/
SPmath = $(source_dir)SPmath.cpp $(include_dir)SPmath.h 
SPmatrix = $(source_dir)SPmatrix.cpp $(include_dir)SPmatrix.h
SPGIO = $(source_dir)mySPGPIO.cpp $(include_dir)mySPGPIO.h $(include_dir)SPGPIO.h

main : mySPGPIO.o
	g++ -o main.out ../lab.cpp $(objects)

mySPGPIO.o : $(SPGIO)
	g++ -c $(SPGIO)

.PHONY : clean
clean :
	rm main.out $(objects)
