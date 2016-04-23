CC = gcc
CFLAGS = -pthread -lrt
DISTDIR = lab4-johnstorch
default: addtest sltest

sltest: sltest.c
	$(CC) $(CFLAGS) sltest.c -o $@

addtest: addtest.c
	$(CC) $(CFLAGS) addtest.c -o $@

dist: lab4-johnstorch.tar.gz

FILES = addtest.c Makefile answers.txt sltest.c SortedList.h

lab4-johnstorch.tar.gz: $(FILES)
	tar -czf $@ --transform='s,^,$(DISTDIR)/,' $(FILES)

clean:
	rm -f *.o *~ *tar.gz addtest sltest
