CC = gcc

TARGET = csv

OBJS = main.o csv.o

CFLAGS = -Wall -std=c99 -Wextra -Werror

Links = -Llib -l3man -lc3sv

SRC := src/csv.c
OBJ := $(SRC:.c=.o)
LIB := lib/lc3sv.a

all: $(LIB) $(TARGET)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(Links)

clean:
	rm -f src/*.o