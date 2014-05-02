#include "key.h"

/* 
 * this is the constructor of the class key, it aims is to generate the key
 * and then store the key in a file
 */
Key::Key() {
    char* key;
    int i, b;
    b=key_size;
    key=new char[key_size];
    RAND_bytes(key,b);
#ifdef _DEBUG
    for(i=0; i<b; i++) 
        printbyte(key[i]);
    //printbyte(key[b-1]);
    printf("\n");
#endif
    writeFile("key.txt");
    delete(key);
}

/* 
 * Allocate the context for decryption
 */
void Key::contextDecryptAllocation() {
    
    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit(ctx,EVP_des_ecb(),NULL,NULL);
    EVP_DecryptInit(ctx,NULL,key,NULL);
    EVP_CIPHER_CTX_set_key_length(ctx,keySize);
    
}

/*
 * Allocate the context for encryption
 */
void Key::contextEncryptionAllocation() {
    
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit(ctx,EVP_des_ecb(),NULL,NULL);
    EVP_EncryptInit(ctx,NULL,key,NULL);
    EVP_CIPHER_CTX_set_key_length(ctx,keySize);
    
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
char* Key::secretEncrypt(char* buffer, int* size) {
    
    //temporary buffer used for decryption
    char* crbuf = new char[*size + EVP_CIPHER_CTX_block_size(ctx)];
    int byteo, pos, byteof, tot;               //output byte  
    EVP_EncryptUpdate(ctx, crbuf, &byteo, buffer, *size);
    pos=byteo;
    EVP_EncryptFinal(ctx, &crbuf[pos], &byteof);
    tot=byteo+byteof;
    *size=tot;
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
char* Key:secretDecrypt(char* buffer, int* size) {
    
    char* debuffero = new char* [*size + EVP_CIPHER_CTX_block_size(ctx)];
    int byteo, byteof, tot;             //output byte
    EVP_DecryptUpdate(ctx, debuffero, &byteo, buffer, *size);
    pos=byteo;
    EVP_DecryptFinal(ctx, &debuffero[pos], &byteof);
    tot=byteo+byteof;
    *size=tot;
    return debuffero;
    
}