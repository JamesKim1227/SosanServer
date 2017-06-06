#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <pthread.h>
#include "worker_thread.h"

typedef struct context Context;
struct context
{
  Worker_Thread **threads;
  int thread_count;
  
  int (*create_thread_pool)(Context *, int);
  void (*add_to_worker_thread)(Context *, int);
};

int context_init(Context *ctx, int thread_count);

#endif