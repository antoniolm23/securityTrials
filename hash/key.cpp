#include "key.h"
#include "util.h"

/* 
 * Constructor of the class
 */
Key::Key() {
    
    OpenSSL_add_all_digests();
    
}


/* 
 * this is the constructor of the class key, it aims is to generate the key
 * and then store the key in a file
 */
void Key::keyGenerator() {
    unsigned char* key;
    int b;
    b=keySize;
    key=new unsigned char[b];
    RAND_bytes(key,b);
#ifdef _DEBUG
    for(i=0; i<b; i++) 
        printbyte(key[i]);
    //printbyte(key[b-1]);
    printf("\n");
#endif
    writeFile("key.txt", key, b);
    delete(key);
}

/* 
 * Allocate the context for decryption
 */
void Key::contextDecryptAlloc() {
    
    int b = keySize;
    ctx=new EVP_CIPHER_CTX;
    unsigned char* key = readKeyFile("key.txt", b);
    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit(ctx, EVP_des_ecb(), NULL, NULL);
    EVP_DecryptInit(ctx, NULL, key, NULL);
    EVP_CIPHER_CTX_set_key_length(ctx,keySize);
    delete(key);
    
}

/*
 * Allocate the context for encryption
 */
void Key::contextEncryptAlloc() {
    
    int b = keySize;
    ctx=new EVP_CIPHER_CTX;
    unsigned char* key = readKeyFile("key.txt", b);
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit(ctx,EVP_des_ecb(),NULL,NULL);
    EVP_EncryptInit(ctx,NULL,key,NULL);
    EVP_CIPHER_CTX_set_key_length(ctx,keySize);
    delete(key);
    
}

/*
 * Encripts the buffer and returns the size of the decrypted buffer and 
 * the encrypted buffer
 * @params:
 *          buffer: the buffer to be encrypted
 *          size: (INOUT) at first is the size of the buffer and then returns 
 *                  the size of the encrypted buffer
 * @returns:
 *          the encrypted buffer
 * NOTE: memory allocation remember to delete
 */
unsigned char* Key::secretEncrypt(const unsigned char* buffer, int* size) {
    
    contextEncryptAlloc();
    //temporary buffer used for decryption
    unsigned char* crbuf = 
        new unsigned char[*size + EVP_CIPHER_CTX_block_size(ctx)];
    int byteo, pos, byteof, tot;               //output byte  
    EVP_EncryptUpdate(ctx, crbuf, &byteo, buffer, *size);
    pos=byteo;
    EVP_EncryptFinal(ctx, &crbuf[pos], &byteof);
    tot=byteo+byteof;
    *size=tot;
    delete (ctx);
    return crbuf;
}

/* 
 * Decrypts the buffer and returns the decrypted buffer along with its size
 * that is represented by the inout parameter size
 * @params:
 *          buffer: the buffer to decrypt
 *          size: (INOUT) dimension of both buffers, at first the one to be 
 *                  decrypted and the decrypted one
 * @return:
 *          the decrypted buffer
 * NOTE: memory allocated here rmemember to delete
 */
unsigned char* Key::secretDecrypt(const unsigned char* buffer, int* size) {
    
    contextDecryptAlloc();
    unsigned char* debuffero =
        new unsigned char [(*size) + EVP_CIPHER_CTX_block_size(ctx)];
    int pos, byteo, byteof, tot;             //output byte
    EVP_DecryptUpdate(ctx, debuffero, &byteo, buffer, *size);
    pos = byteo;
    EVP_DecryptFinal(ctx, &debuffero[pos], &byteof);
    tot = byteo+byteof;
    *size=tot;
    delete (ctx);
    return debuffero;
    
}

/* 
 * Function that generates the hash of a message and returns the result
 * and the size
 * @params:
 *          buffer: buffer on which compute the hash
 *          size: (INOUT) at the beginning size of the message, 
 *                          then size of the hash
 * @returns:
 *          the computed hash 
 */
unsigned char* Key::generateHash(char* buffer, int* size) {
    
    char* alg = "sha1";
    int hashSize, rest;
    unsigned char* hashBuf;
    static const int k = 512;
    
    //context allocation and preparation
    const EVP_MD* md=EVP_get_digestbyname(alg);
    EVP_MD_CTX* mdctx;
    mdctx = new EVP_MD_CTX;
    //context preparation
    EVP_MD_CTX_init(mdctx);
    EVP_DigestInit(mdctx, md);
    //end context allocation
    
    //check the correct allocation
    if(mdctx==NULL) {
        cerr<<"error1 detected\n";
        exit(-1);
    }
    if(md==NULL) {
        cerr<<"error2 detected\n";
        exit(-1);
    }
    
    //allocate buffer
    hashSize = EVP_MD_size(md);
    hashBuf = new unsigned char[hashSize];
    
    //prepare the various integer to compute the hash
    rest = *size % k;
    int ptr = 0;
    
    //BEGIN HASH COMPUTATION
    for(int i = 0; i < (*size)/k; i++) {
        EVP_DigestUpdate(mdctx, &buffer[ptr], k);
        ptr += k;
    }
    
    if(rest) {
        EVP_DigestUpdate(mdctx, &buffer[ptr], rest);
    }
    
    //put the computed hash in hashBuf and size is given
    EVP_DigestFinal_ex(mdctx, hashBuf,(unsigned int*) size);
    //END HASH COMPUTATION
    
    //hash context deallocation
    EVP_MD_CTX_cleanup(mdctx);
    free(mdctx);
    
    return hashBuf;
    
}

/* 
 * Compare the given hash with the computed one to state if the message 
 * has been modified by an adversary
 * NOTE: the buffer and the size passed as parameter includes the computed hash
 * @params:
 *          buffer: given buffer on which compute the hash
 *          size: (INOUT) size of the buffer plus the hash at first, without it
 *                        at the end
 * @return:
 *          true if the hashes are equal, false otherwise
 */
bool Key::compareHash(char* buffer, int* size) {
    
    printByte((unsigned char*)buffer, *size);
    
    char* alg = "sha1";
    unsigned int hashSize, rest;
    unsigned char* hashBuf;
    static const int k = 512;
    bool result = false;
    
    //context allocation and preparation
    const EVP_MD* md=EVP_get_digestbyname(alg);
    EVP_MD_CTX* mdctx;
    mdctx = new EVP_MD_CTX;
    //context preparation
    EVP_MD_CTX_init(mdctx);
    EVP_DigestInit(mdctx, md);
    //end context allocation
    
    //check the correct allocation
    if(mdctx==NULL) {
        cerr<<"error1 detected\n";
        exit(-1);
    }
    if(md==NULL) {
        cerr<<"error2 detected\n";
        exit(-1);
    }
    
    //allocate buffer
    hashSize = EVP_MD_size(md);
    hashBuf = new unsigned char[hashSize];
    
    //since the buffer contains at its end its hash, we need to toggle it
    *size -= hashSize;
    
    //prepare the various integer to compute the hash
    rest = *size % k;
    int ptr = 0;
    
    //BEGIN HASH COMPUTATION
    for(int i = 0; i < (*size)/k; i++) {
        EVP_DigestUpdate(mdctx, &buffer[ptr], k);
        ptr += k;
    }
    
    if(rest) {
        EVP_DigestUpdate(mdctx, &buffer[ptr], rest);
    }
    
    //put the computed hash in hashBuf and size is given
    EVP_DigestFinal_ex(mdctx, hashBuf, &hashSize);
    //END HASH COMPUTATION
    
    //printByte((unsigned char*)hashBuf, hashSize);
    //printByte((unsigned char*)&buffer[*size], hashSize);
    
    if(memcmp(hashBuf, &buffer[*size], hashSize) == 0)
        result = true;
    
    //hash context deallocation
    EVP_MD_CTX_cleanup(mdctx);
    delete(mdctx);
    
    return result;
    
}