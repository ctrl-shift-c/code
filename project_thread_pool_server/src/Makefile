#app:add.c sub.c mul.c des.c main.c
#	gcc add.c sub.c mul.c des.c main.c -I../include -o app

#生成.o核心二进制文件

#app:add.o sub.o mul.o des.o main.o
#	gcc add.o sub.o main.o mul.o des.o -o app

#add.o:add.c
#	gcc add.c -I../include -c
#mul.o:mul.c
#	gcc mul.c -I../include -c
#sub.o:sub.c
#	gcc sub.c -I../include -c
#des.o:des.c
#	gcc des.c -I../include -c
#main.o:main.c
#	gcc main.c -I../include -c



.PHONY:clean install distclean output

CC=gcc
INCLUDE_PATH=../include
LIBRARY_PATH=../lib
LIBRARY_NAME_1=pthread
LIBRARY_NAME_2=g
CFLAGS=-I$(INCLUDE_PATH) -Wall -c
CPPFAGS=-E -D -01
LIBRARY_NAME=-l$(LIBRARY_NAME_1) 
LDFLAGS=-L$(LIBRARY_PATH) $(LIBRARY_NAME)
TARGET=server
SRCFIFENAME=$(wildcard *.c)
DESFIFENAME=$(patsubst %.c,%.o,$(SRCFIFENAME))
CLEAN=clean
RM=sudo rm -rf
INSTALL_PATH=/usr/bin/

$(TARGET):$(DESFIFENAME)
	$(CC) $^ $(LDFLAGS) -o $@

%.o:%.c
	$(CC) $(CFLAGS) $(LDFLAGS)  $<

output:
	@echo "SRCFILE=$(SRCFIFENAME)"
	@echo "DESFIFE=$(DESFIFENAME)"

$(CLEAN):
	$(RM) $(DESFIFENAME) $(TARGET)

install:
	sudo cp $(TARGET) $(INSTALL_PATH)

distclean:
	$(RM) $(INSTALL_PATH)$(TARGET);




