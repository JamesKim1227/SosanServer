
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <apr_dso.h>

#include "context.h"
#include "worker_thread.h"
#include "plugin_handler.h"

Plugin_Handler*
create_plugin_handler(int client_socket)
{
  apr_dso_handle_t *dso_h;
  apr_pool_t *mp;
  apr_status_t rv;
  apr_dso_handle_sym_t init = NULL;
  apr_dso_handle_sym_t request = NULL;
  apr_dso_handle_sym_t destroy = NULL;
  Plugin_Handler *ph = malloc(sizeof(Plugin_Handler));
  char buf[1024];
  
  apr_pool_create(&mp, NULL);
  
  if ((rv = apr_dso_load(&dso_h, "libsample_dso.so", mp)) != APR_SUCCESS)
  {
    printf("apr_dso_load error: %s\n", apr_strerror(rv, buf, 1024));
    return NULL;
  }

  if ((rv = apr_dso_sym(&init, dso_h, "init")) != APR_SUCCESS)
  {
    printf("apr_dso_sym error\n");
    return NULL;
  }
  
  if ((rv = apr_dso_sym(&request, dso_h, "request")) != APR_SUCCESS)
  {
    printf("apr_dso_sym error\n");
    return NULL;
  }
  
  if ((rv = apr_dso_sym(&destroy, dso_h, "destroy")) != APR_SUCCESS)
  {
    printf("apr_dso_sym error\n");
    return NULL;
  }
  
  ph->client_socket = client_socket;
  
  ph->init = init;
  ph->request = request;
  ph->destroy = destroy;
  
  return ph;
}

static void
add_to_worker_thread(Context *ctx, int client_socket)
{
  int i, load_balance_index=0;
  int min_event_count;
  Worker_Thread *w_th;
  
  for (i=0; i<ctx->thread_count; i++)
  {
    if (i == 0)
    {
      min_event_count = ctx->threads[0]->event_count;
      w_th = ctx->threads[0];
      continue;
    }
    
    if ( ctx->threads[i]->event_count < min_event_count )
    {
      w_th = ctx->threads[i];
      load_balance_index = i;
      break;
    }
  }
  
  Plugin_Handler *ph = create_plugin_handler(client_socket);
  if (!ph)
  {
    printf("create plugin handler error\n");
    exit(1);
  }
  ph->init();
  w_th->ev.data.ptr = (void*)ph;
  // w_th->ev.data.fd = client_socket;
  epoll_ctl(w_th->epoll_fd, EPOLL_CTL_ADD, client_socket, &w_th->ev);
  w_th->event_count++;
  
  printf("add_to_worker_thread - end, load_balance_index : %d\n", load_balance_index);
}

static int
create_thread_pool(Context *ctx, int count)
{
  int i;
  Worker_Thread *w_th;
  
  printf("create_thread_pool\n");
  
  for (i=0; i<count; i++)
  {
    printf("create_thread_pool - %d\n", i);
    w_th = (Worker_Thread*)malloc(sizeof(Worker_Thread));
    worker_thread_init(w_th);
    ctx->threads[i] = w_th;
    ctx->thread_count++;
    pthread_create(&ctx->threads[i]->thread, NULL, ctx->threads[i]->run, (void*)i);
  }
  
  printf("create_thread_pool - end\n");
}

int context_init(Context *ctx, int thread_count)
{
  memset(ctx, 0, sizeof(Context));
  
  ctx->threads = (Worker_Thread**)malloc(sizeof(Worker_Thread*) * thread_count);
  memset(ctx->threads, 0, sizeof(ctx->threads));
  
  ctx->thread_count = 0;
  
  ctx->create_thread_pool = create_thread_pool;
  ctx->add_to_worker_thread = add_to_worker_thread;
  
  return 0;
}