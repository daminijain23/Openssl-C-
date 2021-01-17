#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>

#define PORT 8080
#define FAIL    -1

using namespace std;

void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
	cout << "Cert is fine"<< "\n";
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        cout << "Info: No client certificates configured.\n";
}


int main(int count, char *strings[])
{
    SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
    char acClientRequest[1024] = {0};
    int bytes;
    const SSL_METHOD *method;
    int sd;
    struct sockaddr_in addr;
    X509 *cert;
   
    cout << "Initiating openssl \n"; 
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    method = TLS_client_method(); 
    ctx = SSL_CTX_new(method);

    
    sd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
   
    cout << "Creating socket \n";
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror("Connection with socket failed");
        abort();
    }


    cout << "Creating ssl connection \n";
    ssl = SSL_new(ctx);      
    SSL_set_fd(ssl, sd);    
    if ( SSL_connect(ssl) == FAIL ){  
        ERR_print_errors_fp(stderr);
	cout << "failed";
}
    else
    {
        ShowCerts(ssl);       
	cert = SSL_get_peer_certificate(ssl);
        if(cert == NULL)
		cout << "Info: No client certificates configured.\n";
        
	bytes = SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
        buf[bytes] = 0;
	cout << SSL_get_cipher(ssl) << "\n";
	cout << "Connected" << "\n";
        for(int i=0; i<strlen(buf); i++){
            cout << buf[i];
    	}
	
	SSL_free(ssl);        /* release connection state */
    }
    close(server);         /* close socket */
    SSL_CTX_free(ctx);        /* release context */
    return 0;
}
