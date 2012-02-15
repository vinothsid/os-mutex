#include "mymutex.h"



/*
*mythread_mutex initializer
*initializes the mutex structure 
*/
int mythread_mutex_init(mythread_mutex_t* mutex,const mythread_mutexattr_t* attr) {
	mutex->val=1;	
	mutex->flag=-1;
	mutex->mQ=NULL;
} 

/*
*mythread_mutex destroy frees associated mutex structure
*
*/
int mythread_mutex_destroy(mythread_mutex_t* mutex)  {
	mutex->val=10;
	mutex->flag=-1;
	mutex->mQ=NULL;
}

/*
*mythread_mutex_lock is the locking function for the mutex
*
*/
int mythread_mutex_lock(mythread_mutex_t* mutex ) {
	int thresh=0; 
	while(1) {
                while(mutex->val!=1) { //spin lock phase
                        thresh++; 
//			write(1,"\nhere in while",strlen("\nhere in while"));
			if(thresh==100) {
//				write(1,"thresh100",strlen("thresh100"));
				mythread_enter_kernel();
				if( mutex->flag == 0 ) {
					write(1,"\nThread suspending on block\n",strlen("\nThread suspending on block\n"));
					mythread_block(&(mutex->mQ),BLOCKED);
					thresh=0;
				} else {
					mythread_leave_kernel();
				}
			}
                }
		
		if(compare_and_swap(&(mutex->val),0,1))  {
	
			mythread_enter_kernel();
//			write(1,"\nhere in acquire\n",strlen("\nhere in acquire\n"));
			(mutex->flag)++;
			mythread_leave_kernel();
                        return;
                }
       	 }

}

int mythread_mutex_unlock(mythread_mutex_t* mutex) {
	mythread_enter_kernel();
	(mutex->flag)--;
	mutex->val=1;
	mythread_unblock(&(mutex->mQ),BLOCKED);
}
