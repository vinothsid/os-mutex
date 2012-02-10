#include "mymutex.h"

typedef struct mythread_cond_var {
        int count;
        mythread_queue_t cQ;
	int lll_lock_val;
} mythread_cond_t;

typedef unsigned int mythread_condattr_t;

/*
*function to initialise condition variable
*
*/
int mythread_cond_init(mythread_cond_t *cond,
    const mythread_condattr_t *attr);

/*
* function to destroy condition variable
*
*/
int mythread_cond_destroy(mythread_cond_t *cond);

int mythread_cond_wait(mythread_cond_t *cond, mythread_mutex_t *mutex);
    
int mythread_cond_signal(mythread_cond_t *cond);

int mythread_cond_broadcast(mythread_cond_t *cond);


