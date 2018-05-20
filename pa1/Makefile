COMP=clang
TARGET=pa1
CFLAGS=-std=c99 -Wall -pedantic
SRC=$(wildcard *.c)
BUILD=$(SRC:%.c=%.o)

$(TARGET): $(BUILD)
	$(COMP) $(CFLAGS) $(BUILD) -o $(TARGET)

%.o: %.c
	$(COMP) -c -o $@ $(CFLAGS) $^

clean:
	rm -f *.o