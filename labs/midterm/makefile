objects = I2C.o
INC = ../../include/
SRC = ../../source/
I2C = $(INC)I2C.h $(SRC)I2C.cpp

main : $(objects)
	g++ -o main.out lab.cpp $(objects) -lwiringPi

test : $(objects)
	g++ -o test.out test.cpp $(objects) -lwiringPi
		
I2C.o: $(I2C)
	g++ -c $(I2C)

.PHONY : clean
clean :
	rm main.out $(objects)
