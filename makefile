CC = gcc

TARGET = csv

OBJS = main.o csv.o

CFLAGS = -Wall -std=c99 -Wextra -Werror

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(Links)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o