objects = Message.o Packet.o Protocol.o RingBuffer.o Robot.o command.o LidarLite.o I2C.o Dobot.o
INC = ../../include/
SRC = ../../source/
MESSAGE = $(INC)Message.h $(SRC)Message.cpp
PACKET = $(INC)Packet.h $(SRC)Packet.cpp
PROTOCOL = $(INC)Protocol.h $(SRC)Protocol.cpp $(INC)ProtocolDef.h $(INC)ProtocolID.h
RINGBUFFER = $(INC)RingBuffer.h $(SRC)RingBuffer.cpp
ROBOT = $(INC)Robot.h $(SRC)Robot.cpp
DOBOT = $(INC)Dobot.h $(SRC)Dobot.cpp
COMMAND = $(INC)command.h $(SRC)command.cpp
#PANTILT = $(INC)PanTilt.h $(SRC)PanTilt.cpp
LIDARLITE = $(INC)LidarLite.h $(SRC)LidarLite.cpp
I2C = $(INC)I2C.h $(SRC)I2C.cpp

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
	
I2C.o: $(I2C)
	g++ -c $(I2C)

.PHONY : clean
clean :
	rm main.out $(objects)
