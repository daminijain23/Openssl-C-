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

// This is client side code to establish a TLS connection with a server
// If the connection is successful, it will send a number to the server.
// The server will calculate it's hash code using SHA256 and return that hash code to client.
// The client will print the received hash code.

int main(){
    SSL_CTX *ctx;
    SSL *ssl;
    char buf[1024];
    int bytes;
    const SSL_METHOD *method;
    int sd;
    struct sockaddr_in addr;
    X509 *cert;
  
    // Initialise OpenSSL, define TLS protocol for client and create a context 
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    method = TLS_client_method(); 
    ctx = SSL_CTX_new(method);

    // Create a socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror("Connection with socket failed");
        abort();
    }


    //Establish a TLS connection with server
    ssl = SSL_new(ctx);      
    SSL_set_fd(ssl, sd);    
    if ( SSL_connect(ssl) == FAIL ){  
        ERR_print_errors_fp(stderr);
	cout << "failed";
    }
    else
    {
	// Check servers's certificates
	cert = SSL_get_peer_certificate(ssl);
        if(cert == NULL)
		cout << "Info: No client certificates configured.\n";

        else{
		string s;
		cout << "Enter a number : ";
		cin >> s;
		const char *str = s.c_str();
		SSL_write(ssl, str, strlen(str));  // Send number to server
		bytes = SSL_read(ssl, buf, sizeof(buf));    // Read the received hash code
        	buf[bytes] = 0;
        	for(int i=0; i<strlen(buf); i++){
            		cout << buf[i];
    		}
		cout << "\n";
	}
	X509_free(cert);
	
	SSL_free(ssl);        // Release connection state
    }
    close(sd);         // Close socket
    SSL_CTX_free(ctx);        // release context
    return 0;
}
