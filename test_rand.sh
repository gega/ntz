#!/bin/bash

N=$1
M=$2

if [ -z $N ]; then
  echo "Usage $0 number-of-tests [f|p]"
  echo "  run the defined number of tests on [f]uture or [p]ast dates"
  exit 1
fi

# between -757353600 .. 10413820800 (1945..2300)
EMIN=-757353600
EMAX=10413820800
if [ "$M" == "f" ]; then
  EMIN=$(date +%s)
fi
if [ "$M" == "p" ]; then
  EMAX=$(date +%s)
fi

TMP1=$(mktemp)
TMP2=$(mktemp)
for((i=0;i<$N;i++)) do
  tz=$(sort tools/tzlist.txt -R |head -1)
  while [ 1 ]; do
    epoch=$(date +%s -d@$(od -vAn -N5 -tu8 < /dev/urandom|awk '{print int(($1/100)-(2**31))}'))
    if [ $epoch -gt $EMIN ] && [ $epoch -lt $EMAX ]; then
      break
    fi
  done
  ./test "$tz" $epoch   | grep -v dst_rule | grep -v ^tz >$TMP1
  ./tztool "$tz" $epoch | grep -v dst_rule | grep -v ^tz >$TMP2
  cmp -s $TMP1 $TMP2
  if [ $? -ne 0 ]; then
    echo "$tz $epoch # FAIL"
    diff $TMP1 $TMP2
  fi
done
rm -f $TMP1 $TMP2
