#include "util.h"

class Client{
    
    //socket of the client
    int cliSock;
    //address of the client
    sockaddr_in cliAddr;
    //name of the client
    string name;
    
    //file descriptors to be used in the select
    int fdmax;
    fd_set master, read_fds;
    
    //part related to the server
    sockaddr servAddr;
    int servSock;
    
public:
    
    //constructor
    Client(int, const char*, const char* );
    
    //send the message to the server
    bool sendServMsg(message);
    //receive message from the server
    message recvServMsg();
    
    //securityProtocol
    bool securityProtocol();
    
    //receive events from the outside world 
    void receiveEvents();
    
    void parseKeyCommand(char);
    void displayHelp();
    
    //destroyer
    ~Client();
    
};