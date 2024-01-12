IDIR = include
ODIR = obj
SDIR = src
JDIR = java
SCDIR = script

CC = gcc
FLAGS = -Wall -Wextra -fPIC -c -I$(IDIR)

PROG = scheduling
TEST = runtest

# Windows
#LIB = $(JDIR)/libscheduling.dll 

# Linux
LIB = $(JDIR)/libscheduling.so 
JAVA_HOME = /usr/lib/jvm/java-11-openjdk-amd64
JINCLUDES = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux

# MacOS
# LIB = $(JDIR)/libscheduling.dylib 
# JAVA_HOME = /Library/Java/JavaVirtualMachines/jdk-11.jdk/Contents/Home/
# JINCLUDES = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/darwin

EXPE = $(SCDIR)/expe

_DEP = util.h list.h tree.h instance.h schedule.h algo.h
DEP = $(patsubst %,$(IDIR)/%,$(_DEP))

_OBJ = util.o list.o tree.o instance.o schedule.o algo.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: run all test jni expe clean delete deletetest deletelib deleteexpe cleanall memorycheck testmemorycheck

all : $(PROG)

run : all
	./$(PROG) $(in) $(out) $(m) $(pre) $(bal)

test : $(TEST)
	./$(TEST)

jni : $(LIB)

expe : $(EXPE)

clean :
	rm -f $(ODIR)/*.o

delete :
	rm -f $(PROG)

deletetest :
	rm -f $(TEST)

deletelib :
	rm -f $(LIB)

deleteexpe :
	rm -f $(EXPE)

cleanall : clean delete deletetest deletelib deleteexpe

memorycheck : all
	valgrind ./$(PROG) $(in) $(out) $(m) $(pre) $(bal)

testmemorycheck : $(TEST)
	valgrind ./$(TEST)

$(ODIR)/%.o : $(SDIR)/%.c $(DEP)
	$(CC) $(FLAGS) -o $@ $<

$(ODIR)/application_Scheduling.o : $(SDIR)/application_Scheduling.c $(DEP)
	$(CC) $(FLAGS) $(JINCLUDES) -o $@ $<

$(PROG) : $(OBJ) $(ODIR)/main.o
	$(CC) -o $@ $^ -lm

$(TEST) : $(OBJ) $(ODIR)/test.o
	$(CC) -o $@ $^ -lm

$(EXPE) : $(OBJ) $(ODIR)/expe.o
	$(CC) -o $@ $^ -lm

$(LIB) : $(OBJ) $(ODIR)/application_Scheduling.o
	$(CC) -shared -o $@ $^ -lm