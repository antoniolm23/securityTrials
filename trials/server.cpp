#include "server.h"
#include "../../../../../Triennale/III anno/Reti Informatiche/progetto/chat_client.h"

/* 
 * Constructor of the class server
 * @params: 
 *          ip address of the server
 *          port on which the server accepts connections 
 */
Server::Server(char* host, int port) {
    
    //creates the socket
    servSock = socket(AF_INET, SOCK_STREAM, 0);
    
    //filling the sockaddr_in structure
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family=AF_INET;
    servAddr.sin_port=htons(port);
    hostent* he;
    if(inet_aton(host, &servAddr.sin_addr)!=0) {
        he=gethostbyname(host);
        if(!he) {
            cerr<<"Can't solve server name"<<host<<" \n";
            exit(1);
        }
        servAddr.sin_addr =* (struct in_addr*) he->h_addr;
    }

    //changing the socket in listen type
    if(bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr))<0) {
        cerr<<"Error in binding of the socket \n";
        exit(-1);
    }
    if(listen(servSock, 0)<0) {
        cerr<<"Error in listening of the socket\n";
        exit(-1);
    }

    //preparing the file descriptors
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(servSock, &master);
    fdmax=servSock;

    //initialize the string
    name=new string(host);
    
    clientList = new List<Client>();
    
    cout<<"server connected at port "<<port<<" host "<<host<<endl;
    
    
}

/* 
 * Search in the list the client to which belongs the socket
 * @params:
 *          s: socket number
 * @returns
 *          Client if present otherwise 0
 */
Client Server::searchListSocket(int s) {
    
    Client p = clientList.begin();
    Client q = clientList.end();
    
    for(p; p!=q; p++) {
        if(p.clientSock == s) 
            return p;
    }
    
    return 0;
    
}

/* 
 * Receive a message from a client
 * @params
 *          sock: socket of the client to who the server is sending the message
 * @returns
 *          the message received
 */
message RecvClientMsg(int sock) {
    
    Client cl = searchListSocket(sock);
    return receiveMessage(sock, cl.clientAddr);
    
}

/* 
 * Send a client a message
 * @params
 *          sock: socket of the client to who the server is sending
 *          msg: the effective message
 * @returns
 *          how the send went 
 */
bool SendClientMsg(int sock, message msg) {
    
    Client cl = searchListSocket(sock);
    return sendMessage(sock, msg, cl.clientAddr);
}

/* 
 * FIXME and ADD something
 * Accept a new connection by a client and add it in the list if it's not
 * already present in the list
 */
void Server::acceptConnection() {
    
    Client newClient;
    int len;
    
    //allocate the socket for a new client
    newClient.clientSock = accept(servSock, (sockaddr*) 
        &newClient.clientAddr, (socklen_t*)&len);
    
    /* 
     * PART RELATED TO THE INSERTION OF CLIENT NAME and PASSWORD
     */
    message = receiveMessage(newClient.clientSock, 
        &newClient.clientAddr);
    
    
    
    //insert the client in the list (last action to do)
    clientList.push_back(newClient);
    
}

/* 
 * Changes the public and the private key of the server
 */
void Server::changeKey(){

}

/* 
 * A simple help display on stdout
 */
void Server::displayHelp(){
    
    cout<<"Possible commands are:\n"
    <<"'h' displayHelp\n"
    <<"'k' change public and private key\n";
    
}


/* 
 * NOTE: possible commands:
 * 'h': help
 * 'k': changeKeys
 */
void Server::parseKeyCommand(){
    
    char cmd;
    cin>>cmd;
    
    //now we have the command in the cmd variable
    switch(cmd) {
        case 'h':
            displayHelp();
            break;
        case 'k':
            changeKey();
            break;
        default;
    }
    
}

/* 
 * Parse the received message and decide the action to perform
 * The decision is based upon the length of the message, if it's
 * length is equal to the one of the public key then this is the 
 * beginning of the protocol, otherwise check if the client has 
 * reached some point of the protocol 
 */
void Server::parseReceivedMessage(Client cl, message msg) {
}

/* TODO & FIXME
 * Handles the receiving of events from the outside world
 */
void Server::receiveEvents() {
    
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
                
                //checks if there's a new connections
                if(i==servSock) 
                    acceptConnection();
                
                //this means a receiving message
                else {
                    Client cl = searchListSocket(i);
                    message msg = receiveMessage(i, &cl.clientAddr);
                    parseReceivedMessage(cl, msg);
                }
            }
        }
    }
}

/* 
 * Destroyer of the server
 */
Server::~Server(){
    free(name);
    ~clientList();
}

