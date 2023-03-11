#!/bin/bash

find "${1}" -type f -printf "%p\n" | while read path; do
	name=$(basename "$path")
	if [[ ! $name =~ ^\..+\.swp$ ]]; then
		newPath=$(echo $path | cut -d '/' -f 2-)
		newDir=$(dirname $newPath)
		mkdir -p "${2}"/$newDir
		cp $path "${2}"/$newDir
	fi
done

