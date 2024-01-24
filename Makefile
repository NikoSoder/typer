CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lncurses -lmenu -lm

SRCS = main.c quotes.c menu_interface.c
OBJS = $(SRCS:.c=.o)

TARGET = bin/type

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)  # Create the bin directory if it doesn't exist
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
