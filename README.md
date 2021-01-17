# Openssl-C-
This project does following - 
1. Creates a server - client connect over TCP and exchange messages.
    server.cpp - server code
    client.cpp - client code
    
2. Creates a TLS Connection between a server and a client.
    tlsserver.cpp - server code
    tlsclient.cpp - client code
    
3. Sends a number from client to server over TLS connection. The server returns it hash code to the client.
    hashserver.cpp - server code
    hashclient.cpp - client code

Operating System - 
NAME="Ubuntu"
VERSION="18.04.5 LTS (Bionic Beaver)"

OpenSSL Version - 1.1.1

Server and Client are on same machine.

To establish TLS connection, self signed certificate was created using - openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 1 -nodes - key and crt creation
