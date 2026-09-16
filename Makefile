CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2
TARGET = banking_system
SRC = banking_system.c

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

test: $(TARGET)
	./tests/smoke_test.sh

clean:
	rm -f $(TARGET) *.o accounts.dat transactions.dat
