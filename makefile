all:
	gcc -w -c mymutex.c -o mymutex.o
	gcc -w -c mycond.c -o mycond.o
	gcc -w mymutex.o mycond.o mytest.c  mythread.a -o mytest

clean:
	rm mymutex.o mycond.o mytest


