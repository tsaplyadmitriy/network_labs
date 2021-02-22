#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(void) {
    int clientSocket = socket (PF_INET, SOCK_STREAM, 0);
    if (clientSocket ==-1) {
        perror("Failure to create socket");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(5432);
    sa.sin_addr.s_addr =htonl(INADDR_ANY);
    int res = inet_pton(AF_INET,  "172.17.0.1",  &sa.sin_addr);
    int connectCode =connect(clientSocket, (struct sockaddr *)&sa, sizeof(sa));
    if(connectCode ==-1) {
        perror("Failure to connect");
        exit(EXIT_FAILURE);
    }
    
    char line[3];
    line[0] = 'a';
    line[1] = 'b';
    line[2] = 'c';
    
    send(clientSocket, line, sizeof(line), 0);
    
    close(clientSocket);
    return EXIT_SUCCESS;
}
