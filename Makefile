CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = banking_system
SRC = banking_system.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o
