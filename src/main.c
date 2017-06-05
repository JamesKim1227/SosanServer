
#include <stdio.h>

#include <apr_getopt.h>

int main(int argc, char const *argv[]) {
  apr_status_t rv;
  apr_pool_t *mp;

  static const apr_getopt_option_t opt_option[] = {
      /* long-option, short-option, has-arg flag, description */
      { NULL, 'i', TRUE, "configure file" },
      { NULL, 'h', FALSE, "show help" },
      { NULL, 0, 0, NULL },
      };

  apr_getopt_t *opt;
  int optch;
  const char *optarg;

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
        default:
          printf("aaa\n");

      }
  }
  if (rv != APR_EOF) {
      printf("bad options\n");
  }

  apr_terminate();

  return 0;
}
