#--------------------------
# invocazione

# make -> per cercare ed eseguire il file di nome make file nella dir corrente
# make -f nome_file -> per cercare un makefile con un nome specifico

#==========================
# sintassi

# target: dipendenze
#	comando

# dove il target rappresenta lo scopo del comando, ma prima di essere eseguito deve rispettare
# le dipende

# ogni target rappresenta una regola che può essere anche invocata singolarmente

#==========================
# macro

# MACRO = val -> crea una variabile macro di valore val
# $(MACRO) -> serve per fare riferimento al valore

#---------------------------
# esempio di macro per invocare gcc
CC = gcc

# regola che linka il file oggetto in file eseguibile
eseguibile: helloworld.o
	$(CC) helloworld.o -o eseguibile

# regola che compila il file sorgente in file oggetto
helloworld.o: helloworld.c
	$(CC) -c helloworld.c
	
# regole che rimuove tutti i file obj e il file eseguibile
clean:
	rm -f *.o
	rm -f eseguibile
