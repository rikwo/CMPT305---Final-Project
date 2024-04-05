
CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c simulator.c input.c instruction.c dependencytracker.c eventlist.c
OBJS = $(SRCS:.c=.o)
DEPS = input.h simulator.h instruction.h dependencytracker.h eventlist.h

TARGET = simulator

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
