CC = gcc
CCFLAGS = -g -std=c11 -Wall -Werror
LDLIBS = -1m
OBJFILES = $(patsubst %.c, %.o, $(wildcard *.c))
TARGET = proj

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES) $(CCFLAGS) $(LDLIBS)
	rm -f *.o

clean: 
	rm -f *.o $(TARGET)

