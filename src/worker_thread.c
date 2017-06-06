
#include <stdio.h>

#include "context.h"
#include "worker_thread.h"

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
      printf("fd : %d\n", ctx->threads[thread_index]->events[i].data.fd);
      len = read(ctx->threads[thread_index]->events[i].data.fd, buf, 1023);
      printf("len : %d\n", len);
      if (len < 0)
      {
        epoll_ctl(ctx->threads[thread_index]->epoll_fd, EPOLL_CTL_DEL, 
                  ctx->threads[thread_index]->events[i].data.fd, ctx->threads[thread_index]->events);
        close(ctx->threads[thread_index]->events[i].data.fd);
        ctx->threads[thread_index]->event_count--;
      }
      buf[len] = '\0';
      printf("buf : %s\n", buf);
      
      write(ctx->threads[thread_index]->events[i].data.fd, buf, 1024);
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