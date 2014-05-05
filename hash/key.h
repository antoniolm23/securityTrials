#include "util.h"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

#define keySize 16

/* 
 * This file is related to the generation and management of keys
 * NOTE: 
 * Secret is the word used to define symmetric encryption
 * Private and public are the couples used for asymmetric encryption
 */
class Key{
    //file in which there's the key
    //string secretKey;
    EVP_CIPHER_CTX* ctx;
    
    //allocation and preparation of the context
    void contextDecryptAlloc();
    void contextEncryptAlloc();
public:
    //constructor (generates the key)
    Key();
    
    void keyGenerator();
    
    //encrypt and decrypt by means of secret key
    unsigned char* secretDecrypt(const unsigned char*, int*);
    unsigned char* secretEncrypt(const unsigned char*, int*);
    //HASH FUNCTIONS
    unsigned char* generateHash(char*, int*);
    bool compareHash(char*, int* );
};