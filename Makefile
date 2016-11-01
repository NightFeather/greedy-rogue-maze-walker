CC=gcc
OBJS=maze.o path.o


default: main

main: $(OBJS)
	$(CC) -g main.c $(OBJS) -o main

$(OBJS): %.o: %.c
	$(CC) -c $< -o $@

clean:
	rm main $(OBJS)
