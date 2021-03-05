

 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
 



 
#include <netinet/in.h>
#include <arpa/inet.h>
 
#include <sys/types.h>
#include <sys/socket.h>
 
/* Constants */
 
#define PORT 5431
#define BUFF 512
#define MAX_CONN 10
 
/*Idea of  keeping list of users was taken from https://www.cyberforum.ru/c-linux/thread1058803.html*/ 

 
typedef struct client
{
  struct sockaddr_in data;
  char nickname[20];
  
} client_t;





 
char * s_addr_to_string(struct sockaddr_in *address);


 

client_t null = {NULL, 0};

/*Function that transfers address to the readable format*/
char * s_addr_to_string(struct sockaddr_in *address)
{
  char *s = (char *) calloc(24, sizeof(char));
  sprintf(s, "%s:%d", inet_ntoa((*address).sin_addr), (int) ntohs((*address).sin_port));
  return s;
}
 
   client_t  clients[MAX_CONN];
     char* clients_names[MAX_CONN];
     int connections = 0;
     
     
client_t get_client(char* key){
    for(int i = 0;i<MAX_CONN;i++){
        if(strcmp(clients_names[i],key)==0){
            return clients[i];
        }
        
    }
    return null;
    
    
}

void add_client(char* key, client_t client){
    
    clients[connections] = client;
    clients_names[connections] = key;
    
    
    connections++;
    
}

 

 
int main(int argc, char ** argv)
{

    
    
    client_t null = {NULL, '0'};
 
  socklen_t socklen = sizeof(struct sockaddr_in);
 


 
  
  for(int i = 0;i<MAX_CONN;i++){
      clients[i] = null;
      clients_names[i] = 0;
}
  connections = 0;
 
  char buffer[BUFF];
 
  client_t s;
  
  int sockfd, recvlen;
  
  struct sockaddr_in server_addr, client_addr;
 
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    perror("Error");
    return -1;
  }
 
 /*Setting port and ip data for UDP*/
  memset((void *) &server_addr, 0, (size_t) socklen);
 
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(PORT);
 
  if (bind(sockfd, (struct sockaddr *) &server_addr, socklen))
  {
    perror("Error");
    return -1;
  }
 

 
  char* welcome = "You've connected to the server for the first time!";
  char* key;
 
  for (;;)
  {
    printf("test");
 
    if ((recvlen = recvfrom(sockfd, buffer, BUFF, 0, (struct sockaddr *) &client_addr, &socklen)) > 0)
    {
        
        
      if ((s = get_client((key = s_addr_to_string(&client_addr)))).nickname == 0) {
          /*If client has connected for the first time*/
       client_t *cl = (client_t *) malloc(sizeof(client_t));
        (*cl).data = client_addr;
        strcpy((*cl).nickname, buffer);
 
        printf("%s (%s:%d) connected\n", (*cl).nickname, inet_ntoa((*cl).data.sin_addr), (int) ntohs((*cl).data.sin_port));
 
        if (sendto(sockfd, welcome, sizeof(welcome), 0, (struct sockaddr *) &(*cl).data, socklen) == -1)
        {
          perror("Error");
        }
        
        add_client(key, *cl);
      }
      else
      {
          /*If client is already in the list of clients*/
        int outlen = recvlen + strlen(s.nickname) + 2;
        char* temp = (char*) calloc(outlen, sizeof(char*));
 
        buffer[recvlen] = 0;
 
        sprintf(temp, "%s: %s", (s.nickname, buffer));
        printf("%s", temp);
 
        
       /*We broadcast the message to all other clients from the list*/
        for (int i = 0; i < MAX_CONN; i++)
        {
          if (clients[i].nickname != 0)
          {
              
              struct sockaddr* sa = (struct sockaddr*)(&clients[i].data);
            sendto(sockfd, temp, outlen, 0, sa, socklen);
          }
        }
        free(temp);
      }
    }

  }
  
  close(sockfd);
  return 0;
}
