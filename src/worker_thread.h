#ifndef __WORKER_THREAD_H__
#define __WORKER_THREAD_H__

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <pthread.h>

typedef struct worker_thread Worker_Thread;

struct worker_thread
{
  pthread_t thread;
  int epoll_fd;
  struct epoll_event ev;
  struct epoll_event events[1024];
  int event_count;
  
  
  void* (*run)(void *arg);
};

int worker_thread_init(Worker_Thread *w_th);

#endif