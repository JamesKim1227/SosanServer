
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include <apr_getopt.h>
#include <apr_dso.h>

#include "context.h"
#include "listener.h"


Context *ctx;

int
main(int argc, char const *argv[]) {
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
  
  if (rv != APR_EOF)
  {
    printf("bad options\n");
    ret = -1;
    goto exception_handler;
  }
  
  // create thread pool
  printf("creating thread pool...\n");
  ctx = (Context *)malloc(sizeof(Context));
  context_init(ctx, 8);
  ctx->create_thread_pool(ctx, 8);
  
  // create listener
  printf("creating listener...\n");
  pthread_create(&listener, NULL, create_listener_thread, (void*)ctx);
  pthread_join(listener, NULL);
 
exception_handler:
  printf("terminating...\n");
  //apr_dso_unload(dso_h);
  apr_pool_destroy(mp);
  apr_terminate();

  return ret;
}
