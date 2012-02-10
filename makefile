all:
	gcc -c mymutex.c -o mymutex.o
	gcc -w mymutex.o p5.c mythread.a
