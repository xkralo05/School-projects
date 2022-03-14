####################################
# Makefile ifj20 projekt prekladac
# riesitelia: 
# Timotej Ponek xponek00
# Kristian Kralovic xkralo05
# Marek Valko xvalko11
# Marek Precner xprecn00
####################################

PRJ=main
##########################################################
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -g #-O0 ##-lm

all: Scanner.o parser.o $(PRJ)
#$(PRJ) parser.o

$(PRJ): Scanner.o str.o main.o parser.o symtable.o expr.o codegen.o
	$(CC) $(CFLAGS) -o $@ Scanner.o str.o main.o parser.o symtable.o expr.o codegen.o

$(PRJ).o: main.c
	$(CC) $(CFLAGS) -c $<

parser.o: parser.c parser.h Scanner.o Error.h expr.o codegen.o
	$(CC) $(CFLAGS) -c $<

expr.o: expr.c expr.h str.o
	$(CC) $(CFLAGS) -c $<

Scanner.o: Scanner.c Scanner.h str.o
	$(CC) $(CFLAGS) -c $<

codegen.o: codegen.c codegen.h str.o
	$(CC) $(CFLAGS) -c $<

symtable.o: symtable.c symtable.h str.o
	$(CC) $(CFLAGS) -c $<

str.o: str.c str.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	# $(PROGS)
