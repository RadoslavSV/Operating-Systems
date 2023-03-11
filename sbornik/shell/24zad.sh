#!/bin/bash

if [ $# -eq 2 ]; then
	find "${1}" -type f -printf "%p %n\n" 2>/dev/null | awk -v n=${2} '{if ($NF>=n) {print $1} }'	
elif [ $# -eq 1 ]; then
	find "${1}" -type l 2>/dev/null | xargs -I @ file @ | egrep ' broken symbolic link to ' | cut -d ':' -f 1
fi
