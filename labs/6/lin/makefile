objects = mySPGPIO.o 24XXEEPROM.o CD405PLEX.o
INC = ../../../include/
SRC = ../../../source/
SPmath = $(SRC)SPmath.cpp $(SRC)SPmath.h 
SPmatrix = $(SRC)SPmatrix.cpp $(INC)SPmatrix.h
SPGIO = $(SRC)mySPGPIO.cpp $(INC)mySPGPIO.h $(INC)SPGPIO.h
CD405 = $(SRC)CD405PLEX.cpp $(INC)CD405PLEX.h
24XX = $(SRC)24XXEEPROM.cpp $(INC)24XXEEPROM.h

main : $(objects)
	g++ -o main.out ../lab.cpp $(objects) -lwiringPi
	
24XXEEPROM.o : $(24XX)
	g++ -c $(24XX) -lwiringPi

CD405PLEX.o : $(CD405) mySPGPIO.o
	g++ -c $(CD405)
	
mySPGPIO.o : $(SPGIO)
	g++ -c $(SPGIO)

.PHONY : clean
clean :
	rm main.out $(objects)
