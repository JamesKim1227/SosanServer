#ifndef __WORKER_THREAD_H__
#define __WORKER_THREAD_H__

typedef struct worker_thread Worker_Thread;

struct worker_thread
{
  int *user_fds;
  
};

int worker_thread_init(Worker_Thread *w_th);

#endif