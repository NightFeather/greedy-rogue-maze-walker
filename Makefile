CC=gcc
OBJS=maze.o path.o
CFLAGS=-g


default: main

main: $(OBJS)
	$(CC) $(CFLAGS) main.c $(OBJS) -o main

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm main $(OBJS)
