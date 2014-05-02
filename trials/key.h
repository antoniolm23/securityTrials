#include "util.h"

#include <openssl/evp.h>
#include <openssl/rand.h>

#define keySize 16

/* 
 * This file is related to the generation and management of keys
 * NOTE: 
 * Secret is the word used to define symmetric encryption
 * Private and public are the couples used for asymmetric encryption
 */
class Key{
    //file in which there's the key
    string secretKey;
    EVP_CIPHER_CTX* ctx;
    
    //allocation and preparation of the context
    void contexDecryptAlloc();
    void contextEncryptAlloc();
public:
    //constructor (generates the key)
    Key();
    
    //encrypt and decrypt by means of secret key
    char* secretDecrypt(char*, int*);
    char* secretEncrypt(char*, int*);
}