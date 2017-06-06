
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>

#include "listener.h"
#include "context.h"

#define MAX_EVENTS 1024

void*
create_listener_thread(void * arg)
{
  struct sockaddr_in server_addr, client_addr;
  struct epoll_event ev, *events;
  int server_socket, client_socket;
  int epoll_fd;
  int eventn, i, client_len;
  Context *ctx = (Context *)arg;
  
  printf("create_listener_thread\n");
  
  events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * MAX_EVENTS);
  if ((epoll_fd = epoll_create(100)) == -1)
  {
    return NULL;
  }
  
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    return NULL;
  }
  
  memset((void*)&server_addr, 0x00, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(12347);
  
  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
  {
    printf("bind error : %d, %s\n", errno, strerror(errno));
    return NULL;
  }
  
  if (listen(server_socket, 5) == -1)
  {
    return NULL;
  }
  
  ev.events = EPOLLIN;
  ev.data.fd = server_socket;
  
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &ev);
  
  while(1)
  {
    eventn = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (eventn == -1)
    {
      printf("epoll_wait error : %s\n", strerror(errno));
      return NULL;
    }
    
    printf("connected...\n");
    printf("eventn : %d\n", eventn);
    
    for (i=0; i<eventn; i++)
    {
      if (events[i].data.fd == server_socket)
      {
        client_len = sizeof(struct sockaddr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        
        // pass client_socket to worker thread
        printf("add to worker thread...\n");
        ctx->add_to_worker_thread(ctx, client_socket);
      }
    }
  }
  
  close(server_socket);
}