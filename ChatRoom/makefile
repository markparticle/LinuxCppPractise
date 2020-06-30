CC = g++
CFLAGS = -std=c++11

all: Server.o Client.o
	$(CC) $(CFLAGS) Server.o -o chatroom_server
	$(CC) $(CFLAGS) Client.o -o chatroom_client

Server.o: Server.cpp
	$(CC) $(CFLAGS) -c Server.cpp

Client.o: Client.cpp 
	$(CC) $(CFLAGS) -c Client.cpp

clean:
	rm -f *.o chatroom_server chatroom_client