objects = PanTilt.o I2C.o
INC = ../../include/
SRC = ../../source/
PANTILT = $(INC)PanTilt.h $(SRC)PanTilt.cpp
I2C = $(INC)I2C.h $(SRC)I2C.cpp

main : $(objects)
	g++ -o main.out lab.cpp $(objects) -lwiringPi -lpthread
	
PanTilt.o : $(PANTILT) I2C.o
	g++ -c $(PANTILT) -pthread
	
I2C.o: $(I2C)
	g++ -c $(I2C)

.PHONY : clean
clean :
	rm main.out $(objects)
