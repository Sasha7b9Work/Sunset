#pragma once


#ifdef WIN32

typedef unsigned long pthread_t;
typedef struct pthread_attr pthread_attr_t;
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
int pthread_join(pthread_t thread, void **retval);

#endif
