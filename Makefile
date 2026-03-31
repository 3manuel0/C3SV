CC = gcc
TARGET = csv
CFLAGS = -Wall -std=c99 -Wextra -Werror

Links = -L./lib -lc3sv -l3man

SRC := src/csv.c
OBJ := $(SRC:.c=.o)
LIB := lib/libc3sv.a

all: $(LIB) $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	@mkdir -p lib
	$(AR) rcs $@ $^

$(TARGET): main.o $(LIB)
	$(CC) main.o -o $(TARGET) $(Links)

clean:
	rm -f src/*.o *.o $(TARGET) lib/libc3sv.a