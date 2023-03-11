#!/bin/bash

mkdir a b c

find . -mindepth 1 -type f 2>/dev/null | egrep -v '\.sh$' | while read file; do
	lines="$(wc -l < "${file}")"

	if [ "${lines}" -lt "${1}" ]; then
		mv "${file}" a
	elif [ "${lines}" -ge "${1}" ] && [ "${lines}" -le "${2}" ]; then
		mv "${file}" b
	else 
		mv "${file}" c
	fi
done
