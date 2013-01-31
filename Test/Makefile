CC=gcc
#FLAGS=-std=c99 -pedantic -Wall -Werror
NAME=multiServerForked

mainServer: $(NAME).o
	$(CC) $(FLAGS) -o $@ $^

$(NAME).o: $(NAME).c
	$(CC) $(FLAGS) -c $^


