CC=gcc
CFLAGS=-Wall -pg

SOURCES=Compression.c Compress.c SaveHm.c HashTable.c Write.c Decompress.c Decompression.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLES=compress decompress write

.PHONY: all clean

all: $(EXECUTABLES)

compress: Compression.o SaveHm.o HashTable.o Compress.o
	$(CC) $(CFLAGS) -o $@ $^

write: Write.o SaveHm.o HashTable.o 
	$(CC) $(CFLAGS) -o $@ $^

decompress: SaveHm.o HashTable.o Compression.o Decompress.o Decompression.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLES) $(OBJECTS)
