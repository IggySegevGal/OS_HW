# Makefile for the Bank program
CC = g++ 
CFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG
CCLINK = $(CC)
OBJS = ttftps.o classes.o
RM = rm -f
TARGET=ttftps
# Creating the  executable

ttftps: $(OBJS)
	$(CCLINK) $(CFLAGS) $(OBJS) -o ttftps
	
# Creating the object files

ttftps.o: ttftps.cpp classes.h classes.cpp
	$(CCLINK) $(CFLAGS) -c ttftps.cpp

classes.o: classes.h classes.cpp
	$(CCLINK) $(CFLAGS) -c classes.cpp


# Cleaning old files before new make

clean:
	$(RM) $(TARGET) *.o

