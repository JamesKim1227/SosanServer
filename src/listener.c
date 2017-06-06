
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>

#include "listener.h"

int user_fds[1024];

void* create_listener_thread(void * arg)
{
  struct sockaddr_in server_addr, client_addr;
  struct epoll_event ev, *events;
  int server_socket, client_socket;
  int epoll_fd;
  
  printf("create_listener_thread\n");
  
  events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * 20);
  if ((epoll_fd = epoll_create(100)) == -1)
  {
    return NULL;
  }
  
  printf("epoll_fd : %d, events: %p\n", epoll_fd, events);
  
  printf("create_listener_thread - 1\n");
  
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    return NULL;
  }
  
  printf("create_listener_thread - 2\n");
  
  memset((void*)&server_addr, 0x00, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(12346);
  
  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
  {
    printf("bind error : %d\n", errno);
    return NULL;
  }
  
  printf("create_listener_thread - 3\n");
  
  if (listen(server_socket, 5) == -1)
  {
    return NULL;
  }
  
  printf("create_listener_thread - 4\n");
  
  ev.events = EPOLLIN;
  ev.data.fd = server_socket;
  
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &ev);
  memset(user_fds, -1, sizeof(user_fds));
  
  int eventn, i, client_len;
  
  printf("epoll_fd : %d, events: %p\n", epoll_fd, events);
  
  printf("create_listener_thread - 5\n");
  while(1)
  {
    eventn = epoll_wait(epoll_fd, events, 20, -1);
    if (eventn == -1)
    {
      printf("create_listener_thread - 6 : %s\n", strerror(errno));
      return NULL;
    }
    
    for (i=0; i<eventn; i++)
    {
      if (events[i].data.fd == server_socket)
      {
        client_len = sizeof(struct sockaddr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        
        // pass client_socket to worker thread
      }
    }
  }
  
  close(server_socket);
}