#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


ssize_t sendMessageToServer(int sockDesc, struct sockaddr_in serverAddr, char *message, char **errorMessage);

int main (int argc, char* argv[])
{
  int sockDesc;
  sockDesc = socket(AF_INET,SOCK_DGRAM,0);
  if( sockDesc < 0)
  {
    printf ("Error creating socket\n");
    return EXIT_FAILURE;
  }

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  int addressCheck = inet_aton(argv[1],&serverAddr.sin_addr);
  if(addressCheck == 0)
  {
    printf("IP address not valid\n");
    return EXIT_FAILURE;
  }
  long port = strtol(argv[2],NULL,10);
  serverAddr.sin_port = htons((uint16_t)port);

  char message[1024];
  fgets(message,1024,stdin);
  char *error = NULL;
  sendMessageToServer (sockDesc, serverAddr, message, &error);
  if (error != NULL)
  {
    printf("%s", error);
    free(error);
    return EXIT_FAILURE;
  }
  return 0;
}
ssize_t sendMessageToServer(int sockDesc, struct sockaddr_in serverAddr, char *message, char **errorMessage)
{
   size_t messageLength = strlen(message) +1;

  ssize_t sendMessageLength = sendto (sockDesc, message, strlen(message)+1,0,(struct sockaddr *) &serverAddr, sizeof(struct sockaddr_in));
  if(sendMessageLength < 0)
  {
    if(errorMessage != NULL)
    {
      *errorMessage = strdup ("Error sending message\n");
    }
    return -1;
  }
  if(sendMessageLength != messageLength)
  {
    if(errorMessage != NULL)
    {
      *errorMessage = strdup("Message not delivered successfully\n");

    }
    return -1;
  }

  return sendMessageLength;
}
