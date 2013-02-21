CC = gcc
LDFLAGS = -pthread -std=c99

.PHONY = clean

server: main.o file.o common.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
common.o: common.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)

main.o: main.c file.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
	
file.o: file.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
	
clean:
	-rm -rf *.o *.gch
