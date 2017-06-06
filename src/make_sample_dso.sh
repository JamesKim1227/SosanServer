#! /bin/bash -x
gcc -o libsample_dso.so sample_dso.c -shared -fPIC