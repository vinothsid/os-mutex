/*
Single Author info:

username Sourabh Dugar

Group info:

username Vinoth Kumar Chandra Mohan

username Sourabh Dugar

username Surya Prakash Rajagopal
*/

#include<stdio.h>
#include"mythread.h"
#include "mymutex.h"
#include "mycond.h"
#define printOut(string) write(1,string,strlen(string))
int glval,glval1;
mythread_mutex_t lock;
mythread_mutex_t lock1;

mythread_cond_t cond;

mythread_queue_t Q;
char* itoa(int val, int base){
        static char buf[32] = {0};
        int i = 30;
        for(; val && i ; --i, val /= base)
                buf[i] = "0123456789abcdef"[val % base];
        return &buf[i+1];
}


void *func(void *name) {

	printOut((char*)name);
}

void *thread1() {

	printOut(itoa(mythread_self(),10));
	printOut(" Thread1 running\n");
}

void *thread2() {
	printOut(itoa(mythread_self(),10));
	printOut(" Thread2 running\n");

}

void *thread3() {
	printOut(itoa(mythread_self(),10));
	printOut(" Thread3 running\n");

}

/*
void getKeyValue() {
	char *data = (char *)mythread_getspecific(key1);
	printOut("tests mythread_getspecific():Key Value retrieved in thread :");
	printOut(itoa(mythread_self(),10));
	printOut(" Value : " );
	if(data!=NULL)
		printOut(data);
	else
		printOut("Key value not found\n");
	printOut("\n");

}

void deleteKey() {
	mythread_key_delete(key1);

	printOut("After key delete\n");
	char *data = (char *)mythread_getspecific(key1);
	if(data==NULL)
		printOut("tests mythread_delete():Data is NULL\n");
	else
		printOut("This should not be printed\n");

}
*/
void *yieldThread1() {
 int i=0;
        while(i<600) {
        //mythread_mutex_lock(&lock);
//        mythread_mutex_lock(&lock1);
/*
		if(glval==100) {
			mythread_cond_wait(&cond,&lock);
			sleep(1);
		}
*/		
                glval++;
                printOut(itoa(glval,10));
                printOut("\nthread1\n");
                glval1++;
//		mythread_enter_kernel();
//		mythread_unblock(&Q,1);
                printOut(itoa(glval1,10));
                printOut("\n");
        //mythread_mutex_unlock(&lock);
//        mythread_mutex_unlock(&lock1);
                i++;
        }
		
}

void *yieldThread2() {
	int i=0;
	while(i<200) {
	//mythread_mutex_lock(&lock);
//      mythread_mutex_lock(&lock1);

		printOut(itoa(glval,10));
		printOut("\nthread2\n");
/*		if(glval == 100 ) {
			mythread_cond_signal(&cond);
		}
*/
		glval++;
		
                glval1++;
//		mythread_enter_kernel();
//		mythread_block_phase1(&Q,1);
//		sleep(5);
//		mythread_enter_kernel();
//		mythread_block_phase2();
                printOut(itoa(glval1,10));
                printOut("\n");
//	mythread_mutex_unlock(&lock);

	if(i==100)
		sleep(1);
//      mythread_mutex_unlock(&lock1);
		i++;
	}
}

void *exitThread() {
	
	printOut("tests mythread_exit():calling mythread_exit\n");
	mythread_exit(NULL);
	printOut("This should not be printed\n");
}

int main() {

	mythread_setconcurrency(4);
	mythread_mutex_init(&lock,NULL);
	mythread_mutex_init(&lock1,NULL);

	mythread_cond_init(&cond,NULL);
	glval=0;
	glval1=0;
	mythread_t tid1,tid2;
	mythread_queue_t head;
	

	tid1 = malloc(sizeof(struct mythread));
	tid2 = malloc(sizeof(struct mythread));
	mythread_create(&tid1,NULL,yieldThread1,NULL);
	mythread_create(&tid2,NULL,yieldThread2,NULL);
	mythread_join(tid1,NULL);
	mythread_join(tid2,NULL);
/*
	mythread_mutex_destroy(&lock);
	mythread_mutex_init(&lock,NULL);
	mythread_create(&tid1,NULL,yieldThread1,NULL);
	mythread_create(&tid2,NULL,yieldThread2,NULL);
	mythread_join(tid1,NULL);
	mythread_join(tid2,NULL);
*/
	mythread_exit(NULL);			












/*
	mythread_t tid1,tid2,tid3,tid4;

	mythread_create(&tid1,NULL,thread1,NULL);
	mythread_create(&tid2,NULL,thread2,NULL);


	mythread_create(&tid3,NULL,thread3,NULL);


	mythread_join(tid1,NULL);
	mythread_join(tid2,NULL);
	mythread_join(tid3,NULL);

	mythread_create(&tid4,NULL,func,(void *)"first 3 threads joined. Argument passed successfully\n");
	mythread_join(tid4,NULL);

	mythread_create(&tid1,NULL,yieldThread1,NULL);
	mythread_create(&tid2,NULL,yieldThread2,NULL);
//	mythread_join(tid1,NULL);
//	mythread_join(tid2,NULL);

	mythread_create(&tid3,NULL,exitThread,NULL);
	mythread_exit(NULL);

*/
}
