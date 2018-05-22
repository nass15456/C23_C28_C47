# Makefile du generateur

SHELL = /bin/bash
CC = gcc
DEP = gcc -MM
FILES = solution sortie carbone_new.xcsp

EXECS = gen_cnst2_new
OBJS = gen_cnst2_new.o contraint.o resoudre.o

FLAGS = -Wall -std=c99 -g -lm
RM = rm -f


all :: $(EXECS)

gen_cnst2_new : $(OBJS)

$(EXECS) :
	$(CC) $^ $(FLAGS) $(LIBS_I) -o $@ $(LIBS)
	
%.o : %.c
	$(CC) -c $(FLAGS) $(LIBS_I) $*.c
	
depend ::
	$(DEP) $(FLAGS) *.c >|.depend
	
clean ::
	$(RM) $(OBJS) $(EXECS) $(FILES)
	
-include .depend
