COMP=clang
TARGET=pa2
CFLAGS=-std=c99 -Wall -pedantic -L. -lruntime
SRC=$(wildcard *.c)
BUILD=$(SRC:%.c=%.o)

$(TARGET): $(BUILD)
	$(COMP) $(CFLAGS) $(BUILD) -o $(TARGET)

%.o: %.c
	$(COMP) -c -o $@ $(CFLAGS) $^

clean:
	rm -f *.o
	