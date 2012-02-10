#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <sys/syscall.h>

pthread_cond_t cn=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mu=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t muAD=PTHREAD_MUTEX_INITIALIZER;
int DELETER_PRESENT;

typedef struct list{
char *item;
struct list *next;
}Node;

Node *begin = NULL;
Node *end = NULL;

int searchThreads = 0;
int insertThreads = 0;
int deleteThreads = 0;

bool addToList(char *item){
	bool success = false;
	
	Node *new = NULL;
	new = malloc(sizeof(Node));
	if(new == NULL){
		fprintf(stderr,"addToList: Malloc failed");
		return false;
	}

	char * buf = malloc(strlen(item) + 1);
	if(buf == NULL){
		fprintf(stderr,"addToList: Malloc failed");
		return false;
	}

	strcpy(buf,item);
	new->item = buf;
	new->next = NULL;

	if(begin == NULL || end == NULL){
		begin = new;
		end = new;
		return true;
	}

	end->next = new;
	end = end->next;
	return true;
}

void printList(){
	if(begin == NULL)
		return;

	Node *temp = begin;

	printf("==============\n");
	while(temp!=NULL){
		printf("%s\n" , temp->item);
		temp = temp->next;
	}
}

bool removeFromList(char *item){
	if(end == NULL){
		return false;
	}

	Node *temp = begin;
	Node *prev = NULL;

	while(temp != NULL){
		if(strcmp(temp->item,item)== 0){
			if(prev == NULL){ 
				begin = begin->next;
				free(temp->item);
				free(temp);
				return true;
				}
			
			if(temp->next == NULL){ 
				end = prev;
			}

			prev->next = temp->next;
			free(temp->item);
			free(temp);
			return true;
		}	
		prev = temp;	
		temp = temp->next;
	}
	return false;
}

bool searchList(char *item){
	Node * temp;
	temp = begin;
	
	if(temp == NULL){
		return false;
	}

	while(temp != NULL){
		if(strcmp(temp->item,item) == 0){
			return true;
		}
		temp = temp->next;
	}
	return false;
}

void printThreadInfo(char* operation, char* value, bool success, pthread_t tid){
	int len = strlen(value);
	//value[len-1] = '\0'; //remove the endline char
	if(success)
		printf("[%08x]    Success %s [ %s ] Retrievers : %i Adders : %i Deleters : %i\n" ,tid, operation,value,searchThreads,insertThreads,deleteThreads);
	else	
		printf("[%08x]    Fail %s [ %s ] Retrievers : %i Adders : %i Deleters : %i\n" , tid , operation,value,searchThreads,insertThreads,deleteThreads);

}

void* retriever(void* item) {
	char* local_item=(char*)item;
	pthread_mutex_lock(&mu);
	while(deleteThreads) {
		pthread_cond_wait(&cn,&mu);
	}
	searchThreads=searchThreads+1;
	pthread_mutex_unlock(&mu);
	bool status=searchList(local_item);
	pthread_t tid=(pid_t) syscall(SYS_gettid);
	printThreadInfo("search",local_item,status,tid);
	pthread_mutex_lock(&mu);
	searchThreads=searchThreads-1;
	pthread_cond_signal(&cn);
	pthread_mutex_unlock(&mu);	
	free(item);
}
void* adder(void *item) {
	char* local_item=(char*)item;
	pthread_mutex_lock(&muAD);
	insertThreads=insertThreads+1;
	bool status=addToList(local_item);
	pthread_t tid=(pid_t) syscall(SYS_gettid);
	printThreadInfo("insert",local_item,status,tid);
	insertThreads=insertThreads-1;
	pthread_mutex_unlock(&muAD);
	free(item);
}
void* deleter(void* item) {
	char* local_item=(char*)item;
	pthread_mutex_lock(&mu);
	while(searchThreads) 
		pthread_cond_wait(&cn,&mu);
	pthread_mutex_lock(&muAD);
	deleteThreads=deleteThreads+1;
	bool status=removeFromList(item);
	pthread_t tid=(pid_t)syscall(SYS_gettid);
	printThreadInfo("delete",local_item,status,tid);
        pthread_mutex_unlock(&muAD);
	deleteThreads=deleteThreads-1;
	while(searchThreads)
		pthread_cond_signal(&cn);
	pthread_mutex_unlock(&mu);
	free(item);

}

void readAndOperate(char* fileName) {
	FILE* tgtFile=fopen(fileName,"r");
	char str[100];
	memset(str,0,100);
	if(!tgtFile) {
		write(1,"file not present\n",strlen("file not present\n")); exit(1);
	}
	pthread_t t[20];
	int numThreads=0;
	int status;
	while(status!=EOF) {
		memset(str,0,100);
		status=fscanf(tgtFile,"%[^\n]\n",str);
		if(str[0]=='A') {
			char* word=malloc(100*sizeof(char));
			strcpy(word,str+2);
			pthread_create(&t[numThreads],NULL,adder,(void*)word);
			numThreads++;
		}
		if(str[0]=='R') {
			char* word=malloc(100*sizeof(char));
			strcpy(word,str+2);
			pthread_create(&t[numThreads],NULL,retriever,(void*)word);
			numThreads++;

		}
		if(str[0]=='D') {
			char* word=malloc(100*sizeof(char));
			strcpy(word,str+2);
			pthread_create(&t[numThreads],NULL,deleter,(void*)word);
			numThreads++;
		}
		if(str[0]=='M') {
			int i=0;
			while(i<numThreads) {
				pthread_join(t[i],NULL);
				i++;
			}
			numThreads=0;
		}	
	}
}

int main(int argc , char** argv){
	if(argv[1]==NULL) {
		write(1,"please enter the name of input file from command line\n",strlen("please enter the name of input file from command line\n"));
		exit(1);
	}
	readAndOperate(argv[1]);
	printList();
	pthread_exit(NULL);
}

