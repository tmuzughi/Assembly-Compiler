CC = gcc
CFLAGS = -g
TARGET = statSem
OBJS = main.o parser.o scanner.o sem.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

scanner.o: scanner.c
	$(CC) $(CFLAGS) -c scanner.c

sem.o: sem.c
	$(CC) $(CFLAGS) -c sem.c

clean:
	/bin/rm -f *.o $(TARGET)