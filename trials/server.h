/*
 *This is the server header file, the server is written in C++ and works 
 *on IPv4 only  
*/
#include "util.h"

class Server {
    
    //string that represents the path to the key
    string keyPath;
    sockaddr_in servAddr;
    //standard socket on which the server will accept connections
    int servSock;
    //path to the resource addressed by the server
    string resPath;
    //file descriptor integers to be managed with the select
    int fdmax;
    fd_set master, read_fds;
    string name;
    
    list<clientInfo> clientList;
    
    //PRIVATE FUNCTIONS
    void parseKeyCommand();
    void changeKey();
    void displayHelp();
    clientInfo searchListSocket(int);
public:
    
    Server(const char*, int);
    
    //accept a connection by a client
    void acceptConnection();
    
    /* 
     * handles the receiving of events coming from outside 
     * e.g. keyboard or socket
     */
    void receiveEvents();
    
    //send and receive message to and from a client
    message RecvClientMsg(int);
    bool SendClientMsg(int, message);
    
    /*parses the received message in order to take the right decision*/
    void parseReceivedMessage(clientInfo, message);
    
    //destroyer
    ~Server();
};
