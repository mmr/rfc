# $Id: Makefile,v 1.2 2002/05/27 01:24:50 binary Exp $

OBJS=rfc.o
CC=gcc
DEFS=
CFLAGS=-O2 -Wall
INC=-I/usr/local/include/postgresql
LIB=-L/usr/local/lib -lpq
CONF= rfc.h

all: rfc

rfc: $(OBJS)
	$(CC) $(CFLAGS) $(DEFS) $(OBJS) $(LIB) -o $@

rfc.o: rfc.c $(CONF)
rfc.o: rfc.c
	$(CC) $(CFLAGS) $(DEFS) $(INC) -c rfc.c

distclean: clean
	rm -f *~

clean:
	rm -f $(OBJS) $(HDRS) rfc rfc.core
