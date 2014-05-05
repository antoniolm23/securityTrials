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

//file management for the key
unsigned char* readKeyFile(const char*, int);

//file management
void writeFile(const char*, unsigned char*, int);
char* readFile(const char*, int*);

//printbyte
void printByte(unsigned char*, int);