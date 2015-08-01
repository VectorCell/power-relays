CFLAGS=-g -std=c99 -O3 -Wall

main : main.o Makefile
	gcc $(CFLAGS) -o main main.o -lwiringPi

%.o : %.c Makefile
	gcc $(CFLAGS) -MD -c $*.c

test : Makefile main
	sudo ./main

clean :
	rm -f *.d
	rm -f *.o
	rm -f main

-include *.d
