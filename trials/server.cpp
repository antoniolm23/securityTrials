#include "server.h"
#include "key.h"

/* 
 * Constructor of the class server
 * @params: 
 *          ip address of the server
 *          port on which the server accepts connections 
 */
Server::Server(const char* host, int port) {
    
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
    name = string(host);
    
    clientList = list<clientInfo>();
    
    cout<<"server connected at port "<<port<<" host "<<host<<endl;
    
    
}

/* 
 * Search in the list the client to which belongs the socket
 * @params:
 *          s: socket number
 * @returns
 *          Client if present otherwise 0
 */
clientInfo Server::searchListSocket(int sock) {
    
    list<clientInfo>::iterator p = clientList.begin();
    list<clientInfo>::iterator q = clientList.end();
    clientInfo t;
    
    for(;p!=q; p++) {

        if(p->clientSock == sock) {
            t.Name = p->Name;
            t.clientAddr = p->clientAddr;
            t.clientSock = p->clientSock;
            t.protoStep = p->protoStep;
            return t;
        }
    }
    
    return t;
    
}

/* 
 * Receive a message from a client
 * @params
 *          sock: socket of the client to who the server is sending the message
 * @returns
 *          the message received
 */
message Server::RecvClientMsg(int sock) {
    
    return receiveMessage(sock);
    
}

/* 
 * Send a client a message
 * @params
 *          sock: socket of the client to who the server is sending
 *          msg: the effective message
 * @returns
 *          how the send went 
 */
bool Server::SendClientMsg(int sock, message msg) {
    
    return sendMessage(sock, msg);
}

/* 
 * FIXME and ADD something
 * Accept a new connection by a client and add it in the list if it's not
 * already present in the list
 */
void Server::acceptConnection() {
    
    clientInfo arrivedClient;
    int len;
    
    //allocate the socket for a new client
    arrivedClient.clientSock = accept(servSock, (sockaddr*) 
        &arrivedClient.clientAddr, (socklen_t*)&len);
    
    if(arrivedClient.clientSock > servSock)
        fdmax = arrivedClient.clientSock;
    
    FD_SET(arrivedClient.clientSock, &master);
    arrivedClient.Name = string("\0");
    //this means that the login has to be done
    arrivedClient.protoStep = TODOLOGIN;
    
    //insert the client in the list (last action to do)
    clientList.push_back(arrivedClient);
    
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
        default:
            break;
    }
    
}

/* 
 * Parse the received message and decide the action to perform
 * The decision is based upon the length of the message, if it's
 * length is equal to the one of the public key then this is the 
 * beginning of the protocol, otherwise check if the client has 
 * reached some point of the protocol 
 * @params:
 *          clientInfo structure to modify
 *          message received
 */
void Server::parseReceivedMessage(clientInfo cl, message msg) {
    
    cout<<msg.text<<endl;
    
    if(cl.protoStep == TODOLOGIN) {
        //if the client has issued a login command than the server fills it 
        if(strncmp("login ", msg.text, 6) == 0) {
            
            //the client sends its name along with the command
            //jumps after the command
            msg.text += 6; 
            msg.len -= 6;
            cl.Name = string(msg.text);
            
            list<clientInfo>::iterator p = clientList.begin();
            list<clientInfo>::iterator q = clientList.end();
            
            for(; p!=q; p++) {
                if(p->clientSock == cl.clientSock) {
                    p->Name = cl.Name;
                    p->protoStep = DONELOGIN;
                }
            }
            
        }
    }
    
    else {
        if(strncmp("fireq ", msg.text, 6) == 0) {
            
            //the client sends the filename after the command
            //jumps after the command
            msg.text += 6; 
            msg.len -= 6;
            
            string namefile = string(msg.text);
            cout<<msg.text<<endl;
        }
    }
    
}

/* TODO & FIXME
 * Handles the receiving of events from the outside world
 */
void Server::receiveEvents() {
    
    //infinite loop to accept events
    while(1) {
        
        //cout<<"hello"<<endl;
        
        read_fds=master;
        if(select(fdmax+1, &read_fds, NULL, NULL, NULL)==-1) {
            cerr<<" error in the select"<<name<<" \n";
            exit(1);
        }
        
        /* 
         * roll all the file descriptors and
         * checks if the file descriptor has been set
         */ 
        for(int i=0; i<=fdmax; i++) {
            
            //cout<<"for cycle "<<i<<endl;
            
            //this means keyboard event
            if(FD_ISSET(0, &read_fds)) 
                parseKeyCommand();
            if(FD_ISSET(i, &read_fds)) {
                
                //checks if there's a new connections
                if(i == servSock) {
                    //cout<<"new connection"<<endl;
                    acceptConnection();
                }
                
                //this means a receiving message
                else {
                    cout<<"new message"<<endl;
                    clientInfo cl = searchListSocket(i);
                    message msg = receiveMessage(i);
                    parseReceivedMessage(cl, msg);
                }
            break;
            }
        }
    }
}

/* 
 * Destroyer of the server
 */
Server::~Server(){
    clientList.clear();
}

