CC = gcc
CFLAGS = -g
TARGET = P0
OBJS = P0.o tree.o 

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

P0.o: P0.c
	$(CC) $(CFLAGS) -c P0.c

tree.o: tree.c
	$(CC) $(CFLAGS) -c tree.c

clean:
	/bin/rm -f *.o $(TARGET)