#include "util.h"

/* 
 * Receive a message on the socket sock and returns it
 * @params: 
 *          sock the socket on which receive the message
 * @returns: 
 *          the received message
 * 
 * NOTE: depending on the type of text it's up to the caller to
 * insert the EOS (end of string) if needed, in this way this
 * function is more general 
 */
message receiveMessage(int sock, sockaddr* addr) {
    
    message msg;
    int msg_size;
    socklen_t sizeSockAddr = sizeof(sockaddr);
    
    //receive the message size first
    msg_size = recvfrom(sock, (void*)&msg.len, sizeof(int), 0, 
                        addr, &sizeSockAddr);
    
    /* compares the received size with the expencted one
     * if the sizes don't match return an empty message */
    if(msg_size < sizeof(int)) {
        cerr<<"error in receiveing the size\n";
        msg.len = 0;
        msg.text = new char[1];
        msg.text[0] = '\0';
        return msg;
    }
    
    //prepare the buffer and receive the message
    msg.text = new char[msg.len];
    msg_size = recvfrom(sock, (void*)msg.text, msg.len, 0, addr, 
                        &sizeSockAddr);
    /* compares the received size with the expencted one
     * if the sizes don't match return an empty message */
    
    if(msg_size < msg.len) {
        cerr<<"error in receiveing the size\n";
        msg.len = 0;
        free(msg.text);
        msg.text = new char[1];
        msg.text[0] = '\0';
        return msg;
    }
    cout<<"received message "<<msg.text<<endl;
    
    return msg;
}

/* 
 * Send a message on the socket sock and returns it
 * @params:
 *          sock: the socket on which receive the message
 *          msg: the message
 * @returns:
 *          the effectiveness of the operation
 */
bool sendMessage(int sock, message msg, sockaddr* addr) {
    
    //send the size of the message
    int size = sendto(sock, (void*)&msg.len, sizeof(int), 0,
        addr, sizeof(sockaddr));
    
    if(size < sizeof(int)) {
        cerr<<"error in the len send "<<size<<endl;
        return false;
    }
    
    //send the effective message
    size = sendto(sock, (void*)msg.text, msg.len, 0, 
        addr, sizeof(sockaddr));
    
    if(size < msg.len) {
        cerr<<"error in the text send "<<msg.len<<" "<<size<<endl;
        cerr<<msg.text<<endl;
        return false;
    }
    //cout<<"message sent\n";
    return true;
    
}

/*
 * Write a file with a buffer of a fixed length
 * @params
 *          filename: name of the file
 *          buffer: buffer to write
 *          dim: dimension of the buffer
 */
void writeFile(char* filename, char* buffer, int dim) {
    
    FILE* f = fopen(filename, "w");
    //effective write on the file
    fwrite(buffer, 1, dim, f);
    fclose(f);
    
}

/* 
 * Read the content of the file and put in into a string that will be returned
 * @params
 *          filename: the name of the file
 *          n: the length to be read if known in advance
 * @return
 *          string: the content of the file
 */
string readFile(char* filename, int n) {
    
    int x = n;
    if(x == 0) {
        //TODO read until the end of the file
    }
    char* buffer = new char[n+1];
    FILE* f = fopen(filename, "r");
    fread(buffer, 1, n, f);
    buffer[n] ='\0';
    string content = string(buffer);
    fclose(f);
    return content;
    
}
