#!/bin/bash

touch dict.txt
cnt=1

cat "${1}" | sort | cut -d ':' -f 1 | tr -s ' ' | sed -r "s/(\s*\(.*\)\s*)//g" | uniq | while read name; do # | sed -r 's/(^[A-Za-z\-\s]+)\(*:.*$/\1/g' | uniq | while read name; do
	echo ""$name";"$cnt"" >> dict.txt
	cnt=$(($cnt+1))
done

cat dict.txt | while read line; do
	name=$(echo $line | cut -d ';' -f 1)
	num=$(echo $line | cut -d ';' -f 2)
	newFile=""${2}"/"$num".txt"
	touch $newFile
	cat "${1}" | tr -s ' ' | egrep "$name" >> $newFile
done
