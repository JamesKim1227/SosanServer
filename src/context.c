
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "context.h"
#include "worker_thread.h"

static void*
func(void *arg)
{
  while(1)
  {
    // do nothing
  }
}

static int
create_thread_pool(Context *ctx, int count)
{
  int i;
  Worker_Thread *w_th;
  
  for (i=0; i<count; i++)
  {
    // w_th = (Worker_thread *)malloc(sizeof(Worker_thread));
    pthread_create(&ctx->threads[i], NULL, func, NULL);
  }
}

int context_init(Context *ctx)
{
  memset(ctx, 0, sizeof(Context));
  
  ctx->threads = (pthread_t*)malloc(sizeof(pthread_t) * 8);
  
  ctx->create_thread_pool = create_thread_pool;
  
  return 0;
}