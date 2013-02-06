CC = gcc
LDFLAGS = -pthread -std=c99
TOOLS = ../tools/

.PHONY = clean

server: main.o file.o common.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
common.o: $(TOOLS)common.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)

main.o: main.c $(TOOLS)file.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
	
file.o: $(TOOLS)file.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
	
clean:
	-rm -rf *.o *.gch
