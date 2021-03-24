/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* chfollmann@tsn.at wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return Poul-Henning Kamp
* ----------------------------------------------------------------------------
* Autor: Christoph Follmnann
* Datum: 24.03.2021
* v1.0
* Projekt: openSSL
*/
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>


void handleErrors()
{
    ERR_print_errors_fp(stderr);
    abort();
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, 32))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}


int main (void)
{
    /*
     * Set up the key and iv. Do I need to say to not hard code these in a
     * real application? :-)
     */

    /* A 256 bit key */
    unsigned char *key = (unsigned char *)"BBBBBBBBBBBBBBB";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"BBBBBBBBBBBBBBB";

    /* Message to be encrypted */
    unsigned char *plaintext =
        (unsigned char *)"Schoene Crypto Welt\r\r\r\r\r\r\r\r\r\r\r\r\r";

    /*
     * Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, depending on the
     * algorithm and mode.
     */
    unsigned char ciphertext[32]={
        0xAA, 0xE3, 0x65, 0x27, 0x2C, 0x81, 0x07, 0x8A,
        0xB6, 0x11, 0x6B, 0x36, 0x18, 0x31, 0xD0, 0xF6,
        0xA5, 0xD3, 0xC8, 0x58, 0x7E, 0x94, 0x6B, 0x53,
        0X0B, 0x79, 0x57, 0x54, 0x31, 0x07, 0xF1, 0x5E
    };
    
  //  unsigned char ciphertext[128];
    /* Buffer for the decrypted text */
    unsigned char decryptedtext[128];

    int decryptedtext_len, ciphertext_len;

    /* Encrypt the plaintext */
//    ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv,
//                              ciphertext);
    printf("%d\n",ciphertext_len);
    /* Do something useful with the ciphertext here */
    printf("Ciphertext is:\n");
    BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

    /* Decrypt the ciphertext */
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
                                decryptedtext);

    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';

    /* Show the decrypted text */
    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);


    return 0;
}


