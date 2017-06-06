#! /bin/bash -x
gcc main.c context.c listener.c worker_thread.c -I./ -I../include/apr-1/ -L ../lib -lapr-1 -lpthread
