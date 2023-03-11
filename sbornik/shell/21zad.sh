#!/bin/bash

A=$(mktemp)
a="$(cat "${1}" | egrep "^${2}=" | sed -r "s/^${2}=//g")"
for v in $a; do
	echo $v >> A
done

B=$(mktemp)
b="$(cat "${1}" | egrep "^${3}=" | sed -r "s/^${3}=//g")"
for v in $b; do
	echo $v >> B
done

sed -r -i "s/(^${3}=).*$/\1/g" "${1}"

comm -1 -3 <(sort A) <(sort B) | while read t; do
	sed -r -i "s/^${3}=.*/&$t /g" "${1}"
done

rm A B
