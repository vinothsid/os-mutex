all:
	gcc -c mymutex.c -o mymutex.o
	gcc -c mycond.c -o mycond.o mymutex.o
	gcc -w mymutex.o mycond.o p5.c  mythread.a
