CFLAGS 	:= -ggdb -Wall
CC := g++
DEBUG ?= 

OBJS := server.o

all: security

security: 		$(OBJS) main.cpp
			$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ $(DBFLAGS) 

server.o: 		server.cpp
			g++ -c $(CFLAGS) $(DEBUG) server.cpp
 
