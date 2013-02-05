CC = gcc
LDFLAGS = -pthread -std=c99

.PHONY = clean

server: main.o file.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
main.o: main.c file.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
	
file.o: file.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
	
clean:
	-rm -rf *.o *.gch
