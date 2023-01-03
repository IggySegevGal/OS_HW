# Makefile for the Bank program
CC = g++ 
CFLAGS =-g -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -pthread
CCLINK = $(CC)
OBJS = Bank.o classes.o
RM = rm -f
TARGET=Bank
# Creating the  executable

Bank: $(OBJS)
	$(CCLINK) $(CFLAGS) $(OBJS) -o Bank
	
# Creating the object files

Bank.o: Bank.cpp classes.h classes.cpp
	$(CCLINK) $(CFLAGS) -c Bank.cpp

classes.o: classes.h classes.cpp
	$(CCLINK) $(CFLAGS) -c classes.cpp


# Cleaning old files before new make

clean:
	$(RM) $(TARGET) *.o

