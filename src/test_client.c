#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

void
error_handling(char *message)
{
  printf("%s\n", message);
  exit(1);
}
 
int
main(int argc, char **argv)
{
  int sock;
  struct sockaddr_in serv_addr;
  char message[1024];
  int str_len;
 
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    error_handling("socket() error");
  }
 
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(12347);

  if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
  {
    printf("connect() error : %s\n", strerror(errno));
    return 1;
  }

  while(1)
  {
    fputs("Input message : (q to quit) : ", stdout);
    fgets(message, 1024, stdin);

    if (!strcmp(message, "q\n"))
    {
      break;
    }
    write(sock, message, strlen(message));

    str_len = read(sock, message, 1024-1);
    message[str_len] = 0;

    printf("message from server: %s \n", message);
  }
          
  close(sock);
  return 0;
}