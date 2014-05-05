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
    msg_size = recvfrom(sock, (void*)&msg.len, sizeof(int), MSG_WAITALL, 
                        addr, &sizeSockAddr);
    
    /* compares the received size with the expencted one
     * if the sizes don't match return an empty message */
    if(msg_size < sizeof(int)) {
        cerr<<"error in receiving the size"<<msg.len<<'\n';
        msg.len = 0;
        msg.text = new char[1];
        msg.text[0] = '\0';
        return msg;
    }
    
    //prepare the buffer and receive the message
    msg.text = new char[msg.len];
    msg_size = recvfrom(sock, (void*)msg.text, msg.len, MSG_WAITALL, addr, 
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
    
    //printByte((unsigned char*)msg.text, msg.len);
    //cout<<"received message "<<msg.text<<endl;
    
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
    
    //printByte((unsigned char*)msg.text, msg.len);
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
void writeFile(const char* filename, unsigned char* buffer, int dim) {
    
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
unsigned char* readKeyFile(const char* filename, int n) {
    
    int x = n;
    if(x == 0) {
        //TODO read until the end of the file
    }
    unsigned char* buffer = new unsigned char[n+1];
    FILE* f = fopen(filename, "r");
    fread(buffer, 1, n, f);
    buffer[n] ='\0';
    fclose(f);
    
    return buffer;
    
}

/* 
 * Print the string byte by byte
 * @params:
 *          tmp: the buffer to print
 *          len: the length of the string
 */
void printByte(unsigned char* tmp, int len) {
    for(int i=0; i<len; i++)
        fprintf(stdout, "%i ", tmp[i]);
    cout<<endl;
}

/* 
 * Read a whole file
 * @parmas:
 *          namefile: the name of the file
 *          size: OUT parameter used to return the dimension of the buffer
 * @returns:
 *          buffer that contains the file 
 */
char* readFile(const char* name, int* size) {
    
    int fsize;
    char* fbuffer;
    //open the file a first time to check the length of it
    FILE* fp=fopen(name, "r");
    if(fp == NULL)
        return NULL;
    fseek(fp, 0, SEEK_END);           //reach the end of it
    fsize = ftell(fp);
    fclose(fp);
    
    //open the file to read it and put its content into a buffer
    fp=fopen(name, "r");
    fbuffer=new char[fsize];
    //copy of the buffer
    fread(fbuffer, 1, fsize, fp);
    *size = fsize;
    return fbuffer;
    
}
