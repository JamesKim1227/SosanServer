
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include <apr_getopt.h>

#include "context.h"
#include "listener.h"

#define DEBUG(format, ...) printf(format"[%d],[%s]", __LINE__, __FILE__);

int main(int argc, char const *argv[]) {
  apr_status_t rv;
  apr_pool_t *mp;
  apr_getopt_t *opt;
  int optch;
  const char *optarg;
  int ret = 0;
  pthread_t listener;

  static const apr_getopt_option_t opt_option[] =
  {
    /* long-option, short-option, has-arg flag, description */
    { NULL, 'i', TRUE, "configure file" },
    { NULL, 'h', FALSE, "show help" },
    { NULL, 0, 0, NULL },
  };

  printf("1");
  
  apr_initialize();
  apr_pool_create(&mp, NULL);

  apr_getopt_init(&opt, mp, argc, argv);

  while ((rv = apr_getopt_long(opt, opt_option, &optch, &optarg)) == APR_SUCCESS)
  {
    switch (optch)
    {
      case 'i':
        printf("opt=i, %s\n", optarg);
        break;
      case 'h':
        printf("-i configure file\n");  /* no arg */
        break;
    }
  }
  printf("22222222222222\n");
  
  if (rv != APR_EOF)
  {
    printf("bad options\n");
    ret = -1;
    goto exception_handler;
  }

  printf("333333333333333\n");
  // create thread pool
  Context *ctx = (Context *)malloc(sizeof(Context));
  
  
  printf("44444444\n");
  context_init(ctx);
  
  
  printf("555555555\n");
  ctx->create_thread_pool(ctx, 8);
  
  printf("6666666666\n");
  // create listener
  pthread_create(&listener, NULL, create_listener_thread, NULL);
  
  printf("77777777\n");
  
  pthread_join(listener, NULL);
  

exception_handler:  
  apr_terminate();

  return ret;
}
