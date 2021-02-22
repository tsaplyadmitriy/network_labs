#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


int main(void){
    
    
  
    int serverSocket  = socket(PF_INET,SOCK_STREAM,0);
    if(serverSocket ==-1){
         printf("Failed to create socket");
        exit(EXIT_FAILURE);
        
    }
    
    
    
    struct sockaddr_in sa;
    
    memset(&sa,0,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(5432);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    
    
    int bindCode = bind(serverSocket, (struct sockaddr*) &sa,sizeof(sa));
    
     if(bindCode ==-1){
         printf("Failed to bind socket");
        exit(EXIT_FAILURE);
        
     }
     printf("Server has been started!\n");
     
     int listenCode = listen(serverSocket,10);
    
     if(listenCode ==-1){
         printf("Failed to listen socket");
        exit(EXIT_FAILURE);
        
     }
     
     char buffer[1024];
     
     for(;;){
         
         int connSocket = accept(serverSocket,NULL,NULL);
         
         ssize_t numBytesRcvd = recv(connSocket, buffer, 1024, 0);
          buffer[numBytesRcvd] = '\0';
        
        printf("Message from cilent: %s\n",buffer);
         
         char line[3];
         line[0] = 'A';
         line[1] = 'B';
         line[2] = 'C';
         
         printf("Message to cilent: %s\n",line);
         
         send(connSocket, line, sizeof(line), 0);
         
         
         
         int shutCode = shutdown(connSocket, SHUT_RDWR);
      
         if(shutCode ==-1){
       
          close(connSocket);
          close(serverSocket);
          exit(EXIT_FAILURE);
        
        }
        close(connSocket);
        
        return 0;
         
     }
    
    return 0;
    
    
}
