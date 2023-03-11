#!/bin/bash

# if [ $USER != 'root' ]; then echo NO!; exit 1; fi

files=$(find "${1}" -type f -regextype egrep -regex ".*${3}.*" | cut -d '/' -f 2-)

for v in $files; do
	dr=$(dirname $v)
	mkdir -p "${2}"/"${dr}"
	touch "${2}"/"${v}"
	rm -r "${1}"/"${v}"
done
