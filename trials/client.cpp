#include "client.h"

/* 
 * Client constructor
 * @params:
 *          port: port on which the client is attached to
 *          name: name of the client
 *          server: name of the server
 */
Client::Client(int port, char* name, char* server){
    
    //name of the client
    this.name = new string(name);
    
    //socket of the client
    cliSock = socket(AF_INET, SOCK_STREAM, 0);
    
    //ip address of the client
    bzero(&cliAddr, sizeof(cliAddr));
    cliAddr.sin_family=AF_INET;
    cliAddr.sin_port=htons(port);
    hostent* he;
    if(inet_aton(server, &cliAddr.sin_addr)!=0) {
        he=gethostbyname(server);
        if(!he) {
            cerr<<"Can't solve server name"<<server<<" \n";
            exit(1);
        }
        cliAddr.sin_addr =* (struct in_addr*) he->h_addr;
    }
    
    //preparing the file descriptors
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(servSock, &master);
    fdmax=cliSock;
    
}

/* 
 * Receive a message from the server
 * @return 
 *         the message received
 */
message Client::recvServMsg() {
    
    return receiveMessage(servSock, servAddr);
    
}

/* 
 * Send a message to the server
 * @params
 *          msg: the message the client sends
 * @returns
 *          how the sent went
 */
bool Client::sendServMsg(message msg){
    
    return sendMessage(servSock, msg, servAddr);
    
}



/* 
 * Parse the command received from the keyboard
 */
/* 
 * NOTE Possible commands are:
 * h -> help displayHelp
 * s -> new message to be sent to the server
 * k -> change the couple of public and private key  
 */
void Client::parseKeyCommand() {
    
    char k;
    char* text;
    cin<<k;
    switch(k) {
        case 'h':
            displayHelp();
            break;
        //this means a new message
        case 's':
            cin<<text;
            int len = strlen(text);
            message* msg = new message;
            msg->len = len;
            msg->text = text;
            sendServMsg(msg);
            break;
        
    }
    
}

/* 
 * Receive events from the outside world, server socket or keyboard
 */
void Client::receiveEvents() {
    
    //infinite loop to accept events
    while(1) {
        
        read_fds=master;
        if(select(fdmax+1, &read_fds, NULL, NULL, NULL)==-1) {
#ifdef _DEBUG
            cerr<<" error in the select"<<name<<" \n";
#endif
            exit(1);
        }
        
        /* 
         * roll all the file descriptors and
         * checks if the file descriptor has been set
         */ 
        for(int i=0; i<=fdmax; i++) {
#ifdef _DEBUG
            cerr<<"for cycle "<<i<<endl;
#endif
            //this means keyboard event
            if(FD_ISSET(0, &read_fds)) 
                parseKeyCommand();
            if(FD_ISSET(i, &read_fds)) {
                
                //receive the message from the server and parse it
                if(i == servSock) {
                    
                    message msg = recvServMsg();
                    //parseRecMessage(msg);
                    
                }
                
            }
        }
    }
}







