objects = PanTilt.o
INC = ../../include/
SRC = ../../source/
PANTILT = $(INC)PanTilt.h $(SRC)PanTilt.cpp

main : $(objects)
	g++ -o main.out lab.cpp $(objects) -lwiringPi
	
PanTilt.o : $(COMMAND)
	g++ -c $(COMMAND)

.PHONY : clean
clean :
	rm main.out $(objects)
