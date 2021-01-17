#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 8080


// Server side code to establish a TLS connection

int main(){
    int sock;
    SSL_CTX *ctx;
    const SSL_METHOD *method;

    // Initialise OpenSSL
    SSL_load_error_strings();	
    OpenSSL_add_ssl_algorithms();

    // Define protocol for connection and initialise context
    method = TLS_server_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_ecdh_auto(ctx, 1);

    // Set the cert
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }

    // Set the key
    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
    }

    // Create and bind socket
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
	perror("Unable to create socket");
	exit(EXIT_FAILURE);
    }

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
	perror("Unable to bind");
	exit(EXIT_FAILURE);
    }

    if (listen(sock, 1) < 0) {
	perror("Unable to listen");
	exit(EXIT_FAILURE);
    }

    // Serve the different client threads.
    // For each connecting client, a new ssl object will be created.
    // If TLS cconnection is not successful, an error message will be printed to console.
    while(1) {
        struct sockaddr_in addr;
        uint len = sizeof(addr);
        SSL *ssl;
        const char reply[] = "test\n";

        int client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client);
    }

    close(sock);
    SSL_CTX_free(ctx);
    EVP_cleanup();

    return 0;
}
