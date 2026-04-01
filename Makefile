CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = mycp
SRC = mycp.c

all: $(TARGET)

$(TARGET): $(SRC)
  $(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
  rm -f $(TARGET)

.PHONY: all clean
