#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <netinet/in.h>
#include <list>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
using namespace std;

/*
 * struct that represents a client:
 * name of him, his associated socket and address
 */
struct Client{
    
    string Name;
    int clientSock;
    sockaddr clientAddr;
    int protoStep;
    
};

/* 
 * message structure:
 * length of the message, text of the message
 */
struct message{
    int len;
    char* text;
};

message receiveMessage(int, sockaddr*);
bool sendMessage(int, message, sockaddr*);
