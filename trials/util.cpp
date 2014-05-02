#include "everything.h"

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
    
    //receive the message size first
    msg_size = recvfrom(sock, (void)&msg.len, sizeof(int), 0, 
                        addr, sizeof(addr));
    
    /* compares the received size with the expencted one
     * if the sizes don't match return an empty message */
    if(msg_size != sizeof(int)) {
        cerr<<"error in receiveing the size\n";
        msg.len = 0;
        msg.text = new char[1];
        msg.text[0] = '\0';
        return msg;
    }
    
    //prepare the buffer and receive the message
    msg.text = new char[msg_size];
    msg_size = recvfrom(sock, (void)&msg.text, msg.len, 0, addr, sizeof(addr));
    /* compares the received size with the expencted one
     * if the sizes don't match return an empty message */
    if(msg_size != msg.len) {
        cerr<<"error in receiveing the size\n";
        msg.len = 0;
        free(msg.text);
        msg.text = new char[1];
        msg.text[0] = '\0';
        return msg;
    }
    
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
        addr, sizeof(addr));
    
    if(size < sizeof(int)) {
        return false;
    }
    
    //send the effective message
    size = sendto(sock, (void*)msg.text, msg.len, 0, 
        addr, sizeof(addr));
    
    if(size < msg.len) 
        return false;
    
    return true;
    
}