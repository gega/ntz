#

all:	test tztool

test:	test.c ntz.h
	gcc -Wall -o test test.c

tztool:	tools/tztool.c
	gcc -Wall -o tztool tools/tztool.c
