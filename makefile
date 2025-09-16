
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = magic

all: $(TARGET)

$(TARGET): main.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) main.o
