#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <pthread.h>

typedef struct context Context;
struct context
{
  pthread_t *threads;
  int thread_count;
  
  int (*create_thread_pool)(Context *, int);
  
};

int context_init(Context *ctx);

#endif