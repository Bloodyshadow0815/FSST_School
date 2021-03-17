#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>


void handleErrors()
{
    ERR_print_errors_fp(stderr);
    abort();
}




int main()
{

}
