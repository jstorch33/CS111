CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
LAB = 1
DISTDIR = lab1-$(USER)
CHECK_DIST = ./check-dist

all: simpsh

TESTS = $(wildcard test*.sh)
TEST_BASES = $(subst .sh,,$(TESTS))

TIMETRASH_SOURCES = \
  main.c 
TIMETRASH_OBJECTS = $(subst .c,.o,$(TIMETRASH_SOURCES))

DIST_SOURCES = \
  $(TIMETRASH_SOURCES) Makefile \
  $(TESTS) check-dist README execline1 execline2 execline3

simpsh: $(TIMETRASH_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(TIMETRASH_OBJECTS)

dist: $(DISTDIR).tar.gz

$(DISTDIR).tar.gz: $(DIST_SOURCES) check-dist
	rm -fr $(DISTDIR)
	tar -czf $@.tmp --mode=u+w --transform='s,^,$(DISTDIR)/,' \
	  $(DIST_SOURCES)
	$(CHECK_DIST) $(DISTDIR)
	mv $@.tmp $@

Skeleton: $(DIST_SOURCES)
	$(MAKE) CHECK_DIST=: USER=$@ lab1-$@.tar.gz

check: $(TEST_BASES)

$(TEST_BASES): simpsh
	./$@.sh

clean:
	rm -fr *.txt *.o *~ *.bak *.tar.gz core *.core *.tmp simpsh $(DISTDIR)

.PHONY: all dist check $(TEST_BASES) clean Skeleton
