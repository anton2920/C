#!/bin/bash

# PROG=./*.out
PROG=./solution.py

OKTESTS=0
ALLTESTS=0

for test in tests/[0-9][0-9] ; do
	TEST=$(basename $test)

	printf "Testing '%s' with '%s'... " $PROG $test
	timeout --foreground 1 $PROG < "$test" > "$TEST".a 2>/dev/null
	if [ $? -eq 124 ] ; then
		printf "FAIL: timeout\n"
	else
		diff "$TEST".a "$test".a > /dev/null
		if [ $? -eq 0 ] ; then
			printf "OK\n"
			((++OKTESTS))
		else
			printf "FAIL: wrong answer\n"
		fi
	fi

	((++ALLTESTS))

	# Remove test artifact
	rm -f "$TEST".a
done

printf "\n=> Passed %d/%d\n" $OKTESTS $ALLTESTS
