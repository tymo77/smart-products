objects = Message.o Packet.o Protocol.o RingBuffer.o Robot.o command.o
INC = ../../../include/
SRC = ../../../source/
MESSAGE = $(INC)Message.h $(SRC)Message.cpp
PACKET = $(INC)Packet.h $(SRC)Packet.cpp
PROTOCOL = $(INC)Protocol.h $(SRC)Protocol.cpp $(INC)ProtocolDef.h $(INC)ProtocolID.h
RINGBUFFER = $(INC)RingBuffer.h $(SRC)RingBuffer.cpp
ROBOT = $(INC)Robot.h $(SRC)Robot.cpp $(INC)Dobot.h
COMMAND = $(INC)command.h $(SRC)command.cpp


#main : 
#	g++ -o main.out ../lab6_main.cpp $(COMMAND) $(ROBOT) $(RINGBUFFER) $(PROTOCOL) $(PACKET) $(MESSAGE) -lwiringPi


main : $(objects)
	g++ -o main.out ../lab6_main.cpp $(objects) -lwiringPi
	
command.o : $(COMMAND)
	g++ -c $(COMMAND)
	
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

.PHONY : clean
clean :
	rm main.out $(objects)
