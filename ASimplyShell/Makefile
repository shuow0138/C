CC = gcc 

CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
	-pedantic-errors -fstack-protector-all

PROGS = d8sh 

.PHONY: all clean

all: $(PROGS)

clean:
	rm -f *.o $(PROGS) *.tmp

$(PROGS): 
d8sh: d8sh.o executor.o lexer.o parser.tab.o 
	$(CC) -o d8sh d8sh.o executor.o lexer.o parser.tab.o -lreadline

executor.o: executor.h command.h
lexer.o: lexer.h
parser.tab.o: parser.tab.h 
d8sh.o: lexer.h executor.h
