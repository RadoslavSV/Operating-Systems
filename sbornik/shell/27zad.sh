#!/bin/bash

links=$(find "${1}" -type l)
cnt=0
for v in $links; do
	cmd=$(file "${v}")
	if [[ "$cmd" =~ broken ]]; then
		cnt=$(($cnt+1))
	else
		dest=$(readlink -f "${v}")
		if [ $# -eq 2 ]; then
			echo "${v} -> ${dest}" >> "${2}"
		else
			echo "${v} -> ${dest}"
		fi
	fi
done

if [ $# -eq 2 ]; then
	echo "Broken symlinks: $cnt" >> "${2}"
else
	echo "Broken symlinks: $cnt"
fi
