programma: main.o generatore.o elaboratore.o analizzatore.o
	gcc -o programma main.o generatore.o elaboratore.o analizzatore.o

main.o: main.c
	gcc -c -o main.o   main.c

generatore.o: generatore.o
	gcc -c generatore.c

analizzatore.o: analizzatore.o
	gcc -c analizzatore.c

elaboratore.o: elaboratore.o
	gcc -c elaboratore.c

semafori.o: semafori.c semafori.h
	gcc -c -o semafori.o  semafori.c

clean:
	rm -f *.o
	rm -f programma