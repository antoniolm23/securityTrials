#pragma once
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
#include <stdio.h>
#include <fstream>

#define TODOLOGIN -1
#define DONELOGIN 0 

using namespace std;

/*
 * struct that represents a client:
 * name of him, his associated socket and address
 */
struct clientInfo{
    
    string Name;
    sockaddr * clientAddr;
    int clientSock;
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
//message management
message receiveMessage(int, sockaddr* = 0);
bool sendMessage(int, message, sockaddr* = 0);

//file management
void writeFile(char*, char*, int);
string readFile(char*);