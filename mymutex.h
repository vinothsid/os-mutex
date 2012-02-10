#ifndef MYMUTEX_H_
#define MYMUTEX_H_

#include "mythread.h"

#define BLOCKED 1
typedef struct mythread_mutex { /*mutex structure */
        int val;
        int flag;
        mythread_queue_t mQ;
} mythread_mutex_t;


typedef unsigned int mythread_mutexattr_t;

/*
*mythread_mutex initializer
*initializes the mutex structure 
*/
int mythread_mutex_init(mythread_mutex_t*,const mythread_mutexattr_t*);

/*
*mythread_mutex destroy frees associated mutex structure
*
*/
int mythread_mutex_destroy(mythread_mutex_t*);

/*
*mythread_mutex_lock is the locking function for the mutex
*
*/
int mythread_mutex_lock(mythread_mutex_t* );

/*
*function to unlock the lock associated with the mutex structure
*
*/
int mythread_mutex_unlock(mythread_mutex_t* );

#endif
