#ifndef __PLUGIN_HANDLER_H__
#define __PLUGIN_HANDLER_H__

typedef struct plugin_handler Plugin_Handler;

struct plugin_handler
{
  int client_socket;
  
  int (*init)(void);
  char* (*request)(void);
  int (*destroy)(void);
};

int plugin_handler_init(Plugin_Handler *ph);

#endif