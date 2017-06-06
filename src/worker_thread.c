
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "context.h"
#include "worker_thread.h"
#include "plugin_handler.h"

extern Context *ctx;

static void*
run(void *arg)
{
  char buf[1024] = {0,};
  int eventn, i;
  int thread_index = (int)arg;
  printf("starting worker thread... - %d\n", thread_index);
  
  while(1)
  {
    eventn = epoll_wait(ctx->threads[thread_index]->epoll_fd, ctx->threads[thread_index]->events, 1024, -1);
    if (eventn == -1)
    {
      printf("worker thread epoll_wail error\n");
      return NULL;
    }
    
    printf("[%d]worker_thread: eventn : %d\n", thread_index, eventn);
    
    int len;
    for (i=0; i<eventn; i++)
    {
      Plugin_Handler *ph = ctx->threads[thread_index]->events[i].data.ptr;
      printf("fd : %d\n", ph->client_socket);
      len = read(ph->client_socket, buf, 1023);
      printf("len : %d\n", len);
      
      if (len < 0)
      {
        epoll_ctl(ctx->threads[thread_index]->epoll_fd, EPOLL_CTL_DEL, 
                  ph->client_socket, ctx->threads[thread_index]->events);
        close(ph->client_socket);
        ctx->threads[thread_index]->event_count--;
        free(ctx->threads[thread_index]->events[i].data.ptr);
      }
      
      // plugin handler
      buf[len] = '\0';
      printf("buf : %s\n", buf);
      
      printf("###\n");
      printf("ph->request : %s\n", ph->request());
      
      strcpy(buf, ph->request());
      
      printf("### buf : %s\n", buf);
      
      write(ph->client_socket, buf, 1024);
    }
  }
  
  return NULL;
}


int
worker_thread_init(Worker_Thread *w_th)
{
  w_th->run = run;
  
  if ((w_th->epoll_fd = epoll_create(100)) == -1)
  {
    return 1;
  }
  
  w_th->ev.events = EPOLLIN;
  
  return 0;
}