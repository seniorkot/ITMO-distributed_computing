COMP=clang
TARGET=pa2
CFLAGS=-std=c99 -Wall -pedantic
SRC=$(wildcard *.c)
BUILD=$(SRC:%.c=%.o)

$(TARGET): $(BUILD)
	$(COMP) $(CFLAGS) -L./lib64 -lruntime $(BUILD) -o $(TARGET)

%.o: %.c
	$(COMP) -c -o $@ $(CFLAGS) $^

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f *.log
	