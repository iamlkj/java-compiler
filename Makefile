TARGET = parser
OBJECT = lex.yy.c y.tab.c y.tab.o lex.yy.o y.output parser y.tab.h main.o semcheck.o symtab.o
CC = gcc -g
LEX = flex
YACC = yacc -v -t -l 
YACCFLAG = -d
LIBS = -lfl -lm

.PHONY:clean

all: $(TARGET)
	
parser: y.tab.o lex.yy.o main.o semcheck.o symtab.o
	$(CC) -o $(TARGET) y.tab.o lex.yy.o main.o semcheck.o symtab.o $(LIBS)

%.o: %.c
	$(CC) -c $<

y.tab.c: parser.y
	$(YACC) $(YACCFLAG) parser.y

lex.yy.c: lex.l
	$(LEX) lex.l

clean:
	rm -f $(TARGET) $(OBJECT)

run:
	./$(TARGET) ./ReleasedPattern/semtest1.p
	./$(TARGET) ./ReleasedPattern/semtest2.p
	./$(TARGET) ./ReleasedPattern/semtest3.p
	./$(TARGET) ./ReleasedPattern/semtest4.p
	./$(TARGET) ./ReleasedPattern/semtest5.p
	./$(TARGET) ./ReleasedPattern/test.p
