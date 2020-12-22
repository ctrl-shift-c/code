.PHONY:CLEAN OUTPUT
CC=gcc
TARGET=process_copy
INCLUDE_PATH=../include
CFLAGS=-I$(INCLUDE_PATH) -Wall -c

SRCFILENAME=$(wildcard *.c)
DESFILENAME=$(patsubst %.c, %.o, $(SRCFILENAME))

$(TARGET):$(DESFILENAME)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) $(CFLAGS) $<
OUTPUT:
	@echo $(SRCFILENAME)
	@echo $(DESFILENAME)
CLEAN:
	rm $(DESFILENAME) $(TARGET)
