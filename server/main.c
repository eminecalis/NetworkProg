#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 2345
int main (int argc, char* argv[])
{
  int socketFd;
  long portLong;

  if (argc < 2)
  {
    printf("Usage: %s HOST [PORT]\n", argv[0]);
    return EXIT_FAILURE;
  }

  if (argc > 2)
  {
    portLong = strtol(argv[2],NULL,10);

    if(portLong < 0 || portLong > 65535)
    {
      portLong = PORT;
      printf("Given port invalid! Falling back to default port\n");
    }
  } else {
    portLong = PORT;
  }
  uint16_t port = (uint16_t) portLong;

  socketFd = socket(AF_INET, SOCK_DGRAM, 0);
  if(socketFd < 0)
  {
    printf("Error creating socket\n");
    return EXIT_FAILURE;
  }

  struct sockaddr_in serverAddr;
  memset(&serverAddr,0,sizeof(struct sockaddr_in));
  serverAddr.sin_family = AF_INET;
  int addressResult = inet_aton (argv[1],&serverAddr.sin_addr);
  if(addressResult < 0)
  {
    printf("Not valid IP address\n");
  }

  // error check fuer 65535




  serverAddr.sin_port = htons(port);

  //bind
  int bindResult;
  bindResult = bind(socketFd,(struct sockaddr*) &serverAddr,sizeof(struct sockaddr_in));
  if(bindResult < 0)
  {
    printf("Error binding socket\n");
  }

  //receive
  char packetBuffer[1500];   //groesse eines UDP Packets
  socklen_t length;
  struct sockaddr_in clientAddr;
  memset(&clientAddr,0,sizeof(struct sockaddr_in));
  ssize_t count = recvfrom (socketFd,packetBuffer,1500,0,(struct sockaddr *) &clientAddr, &length);
  if(count < 0)
  {
    printf("Error while reading from socket\n");
  }

  char *host = inet_ntoa (clientAddr.sin_addr);
  uint16_t clientPort = ntohs(clientAddr.sin_port);

  printf("Message sent from: %s from port: %hu\n", host, clientPort);

  printf ("%s\n",packetBuffer);

  return EXIT_SUCCESS;
}
