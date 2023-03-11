#!/bin/bash

if [ -d "${1}" ]; then
	find "${1}" -type l 2>/dev/null | while read link; do
		res=$(file "${link}")

		if [[ "${res}" =~ ^.*:\ broken\ symbolic\ link\ to\ .*$ ]]; then
			echo "${link}"
		fi
	done
fi
