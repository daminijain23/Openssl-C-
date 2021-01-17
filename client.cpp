#include <iostream> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 

using namespace std;

int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char hello[] = "Hello from client"; 
    char buffer[1024] = {0}; 
    
    // Create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        cout << "\n Socket creation error \n"; 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        cout << "\nInvalid address/ Address not supported \n"; 
        return -1; 
    } 
  
    // Connect to server 
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout << "\nConnection Failed \n"; 
        return -1; 
    } 

    // Send Message
    send(sock , hello , strlen(hello) , 0 ); 
    cout << "Hello message sent\n"; 

    // Receive server's message
    valread = read( sock , buffer, 1024); 
    
    for(int i=0; i<strlen(buffer); i++){
            cout << buffer[i];
    }

    return 0; 
} 
