objects = Message.o Packet.o Protocol.o RingBuffer.o Robot.o command.o LidarLite.o I2C.o Dobot.o CameraCom.o SPmath.o SPmatrix.o ServoHat.o mySPGPIO.o SPI_Slave.o DCMotor.o MotorPlate.o
INC = ../../include/
SRC = ../../source/
MESSAGE = $(INC)Message.h $(SRC)Message.cpp
PACKET = $(INC)Packet.h $(SRC)Packet.cpp
PROTOCOL = $(INC)Protocol.h $(SRC)Protocol.cpp $(INC)ProtocolDef.h $(INC)ProtocolID.h
RINGBUFFER = $(INC)RingBuffer.h $(SRC)RingBuffer.cpp
ROBOT = $(INC)Robot.h $(SRC)Robot.cpp
DOBOT = $(INC)Dobot.h $(SRC)Dobot.cpp
COMMAND = $(INC)command.h $(SRC)command.cpp
CAMERACOM = $(INC)CameraCom.h $(SRC)CameraCom.cpp
PANTILT = $(INC)PanTilt.h $(SRC)PanTilt.cpp
LIDARLITE = $(INC)LidarLite.h $(SRC)LidarLite.cpp
I2C = $(INC)I2C.h $(SRC)I2C.cpp
SPmath = $(SRC)SPmath.cpp $(INC)SPmath.h
SPmatrix = $(SRC)SPmatrix.cpp $(INC)SPmatrix.h
SERVOHAT = $(SRC)ServoHat.cpp $(INC)ServoHat.h
MYSPGPIO = $(SRC)mySPGPIO.cpp $(INC)mySPGPIO.h $(INC)SPGPIO.h
SPISLAVE = $(SRC)SPI_Slave.cpp $(INC)SPI_Slave.h
DCMOTOR = $(SRC)DCMotor.cpp $(INC)DCMotor.h
MOTORPLATE = $(SRC)MotorPlate.cpp $(INC)MotorPlate.h

main : $(objects)
	g++ -o main.out main.cpp $(objects) -lwiringPi
	
command.o : $(COMMAND)
	g++ -c $(COMMAND)
	
Dobot.o : $(DOBOT) Robot.o
	g++ -c $(DOBOT)
	
Robot.o : $(ROBOT)
	g++ -c $(ROBOT)

RingBuffer.o : $(RINGBUFFER)
	g++ -c $(RINGBUFFER)
	
Protocol.o : $(PROTOCOL)
	g++ -c $(PROTOCOL)
	
Packet.o : $(PACKET)
	g++ -c $(PACKET)
	
Message.o : $(MESSAGE)
	g++ -c $(MESSAGE)
	
LidarLite.o : $(LIDARLITE) I2C.o
	g++ -c $(LIDARLITE)
	
CameraCom.o : $(CAMERACOM)
	g++ -c $(CAMERACOM)
	
I2C.o: $(I2C)
	g++ -c $(I2C)

SPmath.o : $(SPmath)
	g++ -c $(SPmath)

SPmatrix.o : $(SPmatrix) $(SPmath)
	g++ -c $(SPmatrix)
	
ServoHat.o : $(SERVOHAT)
	g++ -c $(SERVOHAT)

mySPGPIO.o : $(MYSPGPIO)
	g++ -c $(MYSPGPIO)

SPI_Slave.o : $(SPISLAVE)
	g++ -c $(SPISLAVE)

DCMotor.o : $(DCMOTOR)
	g++ -c $(DCMOTOR)

PanTilt.o : $(PANTILT)
	g++ -c $(PANTILT)

MotorPlate.o : $(MOTORPLATE)
	g++ -c $(MOTORPLATE)


.PHONY : clean
clean :
	rm main.out $(objects)
