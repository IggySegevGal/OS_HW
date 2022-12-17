# Makefile for the smash program
CC = g++ 
CFLAGS =-g -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG
CCLINK = $(CC)
OBJS = smash.o commands.o signals.o classes.o
RM = rm -f
TARGET=smash
# Creating the  executable

smash: $(OBJS)
	$(CCLINK) $(OBJS) -o smash
	
# Creating the object files

commands.o: commands.cpp commands.h classes.h classes.cpp
	$(CCLINK) $(CFLAGS) -c commands.cpp
 
smash.o: smash.cpp commands.h classes.h classes.cpp
	$(CCLINK) $(CFLAGS) -c smash.cpp

signals.o: signals.cpp signals.h classes.h classes.cpp
	$(CCLINK) $(CFLAGS) -c signals.cpp

classes.o: classes.h classes.cpp 
	$(CCLINK) $(CFLAGS) -c classes.cpp


# Cleaning old files before new make

clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

