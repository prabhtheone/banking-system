CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2
TARGET = banking_system
SRC = banking_system.c

.PHONY: all test sanitize clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

test: $(TARGET)
	sh tests/smoke_test.sh

sanitize:
	$(CC) $(CFLAGS) -g -fsanitize=address,undefined -fno-omit-frame-pointer -o $(TARGET)-sanitized $(SRC)
	ASAN_OPTIONS=detect_leaks=1 ./$(TARGET)-sanitized </dev/null || true
	rm -f $(TARGET)-sanitized

clean:
	rm -f $(TARGET) *.o accounts.dat transactions.dat
