cmd: shell.o commands.o lib.o signal_handler.o
	gcc shell.o commands.o lib.o signal_handler.o -o cmd
	rm -rf *.o
	./cmd

shell.o: shell.c
	gcc -c shell.c

commands.o: commands.c
	gcc -c commands.c

signal_handler.o: signal_handler.c
	gcc -c signal_handler.c
	
lib.o: lib.c
	gcc -c lib.c

clean:
	rm -rf *.o