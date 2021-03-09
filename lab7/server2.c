 
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
#define BUFLEN 512
#define MAX_CONN 10
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
 
/*Idea of  keeping list of users was taken from https://www.cyberforum.ru/c-linux/thread1058803.html*/ 


int main()
{
  int s;
  struct sockaddr_in server, si_other;
  int recv_len;
  char buf[BUFLEN];
  socklen_t slen;
 

  //slen = sizeof(si_other) ;
  

  //Create a socket
  if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
  {
    printf("Could not create socket ");
  }
  printf("Socket created.\n");
  
  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( PORT );
  
  //Bind
  if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
  {
    printf("Bind failed with error code");
    exit(EXIT_FAILURE);
  }
  puts("Bind done");

    struct sockaddr_in si_clients[4];
    int slen_clients[4];
    int number_of_client = 0;


  //keep listening for data
  while(1)
  {
    printf("Waiting for data...");
    fflush(stdout);
    
    //clear the buffer by filling null, it might have previously received data
    memset(buf, ' ', BUFLEN);
    
    //try to receive some data, this is a blocking call
    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
    {
      printf("recvfrom() failed with error code ");
      exit(EXIT_FAILURE);
    }
        
        // if client is a new one, remember it
        int a = 0;
        for (int i = 0; i < number_of_client; i++){
            if (inet_ntoa(si_other.sin_addr) == inet_ntoa(si_clients[i].sin_addr) && si_other.sin_port == si_clients[i].sin_port){
                a = 1;
                break;
            }
        }
        printf("%d\n", number_of_client);
    if (a == 0){
            si_clients[number_of_client] = si_other;
            slen_clients[number_of_client] = slen;
            number_of_client += 1;
        }
    //print details of the client/peer and the data received
    printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
    printf("Data: %s\n" , buf);
    
    //now reply the client with the same data
        for (int i = 0; i < number_of_client; i++){
            // do not send message to client who sent
            if (!(inet_ntoa(si_other.sin_addr) == inet_ntoa(si_clients[i].sin_addr) && si_other.sin_port == si_clients[i].sin_port)){
                
                if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_clients[i], slen_clients[i]) == SOCKET_ERROR)
                    {
                        
//                         printf("sendto() failed with error code");
//                         exit(EXIT_FAILURE);
                    }
            }
        }
  }

  close(s);
 
  
  return 0;
}
