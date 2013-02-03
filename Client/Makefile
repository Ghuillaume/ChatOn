CC = gcc
LDFLAGS = -pthread -std=c89

.PHONY = clean

client: client.o 
	$(CC) -o $@ $^ $(LDFLAGS)
	
main.o: client.c
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
	
clean:
	-rm -rf *.o *.gch
