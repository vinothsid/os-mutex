#include "mymutex.h"
#include "mycond.h"

static int lll_lock(int*);
static int lll_unlock(int*);


int mythread_cond_init(mythread_cond_t *cond,
    const mythread_condattr_t *attr) {
	cond->count=0;
	cond->lll_lock_val=1;
	cond->cQ=NULL;
}

/*
* function to destroy condition variable
*
*/
int mythread_cond_destroy(mythread_cond_t *cond) {

}

int mythread_cond_wait(mythread_cond_t *cond, mythread_mutex_t *mutex) {
	lll_lock(&(cond->lll_lock_val));
	mythread_enter_kernel();
	mythread_block_phase1(&(cond->cQ),BLOCKED);
	mythread_mutex_unlock(mutex);
	mythread_enter_kernel();
	cond->count++;
	write(1,"\nhere in after count\n",strlen("\nhere in after count\n"));
	lll_unlock(&(cond->lll_lock_val));
	mythread_block_phase2();
	mythread_mutex_lock(mutex);
	write(1,"\nhere in after reacquisition\n",strlen("\nhere in after reacquisition\n"));
}

int mythread_cond_signal(mythread_cond_t *cond) {
	lll_lock(&(cond->lll_lock_val));
	write(1,"\nhere in after lll_lock in cond_singal\n",strlen("\nhere in after lll_lock in cond_singal\n"));
	mythread_enter_kernel();
	mythread_unblock(&(cond->cQ),BLOCKED);
	cond->count--;
	lll_unlock(&(cond->lll_lock_val));
	
	write(1,"\nhere in after unlock of local lock\n",strlen("\nhere in after unlock of local lock\n"));
}

int mythread_cond_broadcast(mythread_cond_t *cond){
	lll_lock(&(cond->lll_lock_val));
	while(cond->count) {
		mythread_enter_kernel();
		mythread_unblock(&(cond->cQ),BLOCKED);
		cond->count--;
	}
	lll_unlock(&(cond->lll_lock_val));
}



/*
*local low level lock based on compare and swap to ensure atomicity in 
*mythread_cond_wait();;
*/

static int lll_lock(int* lll_val) {
        while(1) {
                while(*lll_val!=1) {
                }
                        if(compare_and_swap(lll_val,0,1))  {
                                return;
                        }//spin here
        }
}
/*
*unlocks the low level lock for mythread_cond_wait();
*/
static int lll_unlock(int* lll_val) {
        *lll_val=1;
}
                                
