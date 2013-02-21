CC = gcc
LDFLAGS = -pthread -std=c99
TOOLS = ../tools/

.PHONY = clean

client: client.o common.o
	$(CC) -o $@ $^ $(LDFLAGS)

common.o: $(TOOLS)common.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
	
main.o: client.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
	
clean:
	-rm -rf *.o *.gch
