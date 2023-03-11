#!/bin/bash

mkdir a
mkdir b
mkdir c

files=$(find . -mindepth 1 -maxdepth 1 -type f -printf "%p\n")

for v in $files; do
	lines=$(cat $v | wc -l)
	if [ $lines -lt "${1}" ]; then
		mv $v a
	elif [ $lines -ge "${1}" ] && [ $lines -le "${2}" ]; then
		mv $v b
	else
		mv $v c
	fi
done
