#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
 
#include <netinet/in.h>
#include <arpa/inet.h>
 
#include <sys/types.h>
#include <sys/socket.h>
 
/*Constants*/
 
#define LEN 25
#define PORT 5431
#define BUFF 521
 

 
int main(int argc, char ** argv)
{
  if (argc != 3)
  {
    printf("Exectute process in the following way: %s [ip] [username]\n", argv[0]);
    return -1;
  }
 
  char username[LEN];
  strcpy(username, argv[2]);
 
  int sockfd, n;
  struct sockaddr_in server_addr;
 
  char send[BUFF];
  char recv[BUFF];
 
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    perror("An error occurred while creating socket");
    return -1;
  }
  
 
  memset(&server_addr, 0, sizeof(server_addr));
 
  printf("%s",argv[1]);
  /*We fill up info about our udp connection*/
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  
  server_addr.sin_port = htons(PORT);
  server_addr.sin_family = AF_INET;
 
  sendto(sockfd, username, LEN, 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
 
  
  
  
  
  
  pid_t pid;
 
  if ((pid = fork()) < 0)
  {
    perror("Error");
    return -1;
  }
  /*We fork() a new process to have two processes: one is to send messages to server, other is to receive ones*/
  else if (pid == 0) {
      /*Process to receive messages*/
    for (;;)
    {
        
      n = recvfrom(sockfd, recv, BUFF, 0, NULL, NULL);
      recv[n] = 0;
      
      fputs(recv, stdout);
    }
  }
  else
  {
       /*Process to send messages*/
       
    while (fgets(send, BUFF, stdin) != NULL)
    {
        
        int send_code = sendto(sockfd, send, strlen(send), 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) ;
      if (send_code== -1) {
        perror("Error");
      }
    }
  }
 
  return 0;
}
