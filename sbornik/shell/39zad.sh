#!/bin/bash

mkdir "${2}"
mkdir "${2}/images"
mkdir "${2}/by-date"
mkdir "${2}/by-album"
mkdir "${2}/by-title"

find "${1}" -type f -regextype egrep -regex ^.*\.jpg$ | while read line; do
	name=$(basename "$line")
	header=$(echo "$name" | cut -d '.' -f 1 | sed -r 's/\([^\(\)]*\)//g' | tr -s ' ')
	album=$(echo "$name" | egrep -o '\([^\(\)]*\)[^\(\)]*\.jpg' | sed -r 's/^.*(\(.*\)).*$/\1/g')
	echo "$line" | egrep -q "\(.*\)"
	if [ $? -ne 0 ]; then
		album="misc"
	fi
	date=$(stat --printf "%y\n" "$line" | cut -d ' ' -f 1)
	Hash=$(sha256sum "$line" | cut -c 1-16)
	
	file="$Hash.jpg"
	cp "$line" "${2}/images/$file"
	
	mkdir -p "${2}/by-date/$date/by-album/$album/by-title"
	mkdir -p "${2}/by-date/$date/by-title"
	mkdir -p "${2}/by-album/$album/by-date/$date/by-title"
	mkdir -p "${2}/by-album/$album/by-title"
	mkdir -p "${2}/by-title"

	ln -s ~/${2}/images/$file "${2}/by-date/$date/by-album/$album/by-title/$header.jpg"
	ln -s ~/${2}/images/$file "${2}/by-date/$date/by-title/$header.jpg"
	ln -s ~/${2}/images/$file "${2}/by-album/$album/by-date/$date/by-title/$header.jpg"
	ln -s ~/${2}/images/$file "${2}/by-album/$album/by-title/$header.jpg"
	ln -s ~/${2}/images/$file "${2}/by-title/$header.jpg"

done
