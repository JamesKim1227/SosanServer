
#include <stdio.h>

int
init()
{
  printf("sample_dso - init\n");
  return 0;
}

char*
request()
{
  printf("sample_dso - request\n");
  return "Hello samle dso request.";
}

int
destroy()
{
  printf("sample_dso - destroy\n");
  return 0;
}