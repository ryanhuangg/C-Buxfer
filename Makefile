CC = gcc
CFLAGS = -Wall -Werror -g

all: buxfer test_groups test_users test_xct

buxfer: buxfer.o lists.o lists.h
	$(CC) $(CFLAGS) -o buxfer buxfer.o lists.o

%.o : %.c lists.h
	$(CC) $(CFLAGS) -c $<


test_groups: lists.o  test_groups.o lists.h
	$(CC) $(CFLAGS) -o test_groups test_groups.o lists.o

test_users: lists.o  test_users.o lists.h
	$(CC) $(CFLAGS) -o test_users test_users.o lists.o

test_xct: lists.o  test_xct.o lists.h
		$(CC) $(CFLAGS) -o test_xct test_xct.o lists.o -lm

clean: 
	rm buxfer *.o test_groups test_users test_xct
