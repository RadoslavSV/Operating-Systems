#!/bin/bash

sites=$(cat "${1}" | cut -d ' ' -f 2 | sort | uniq -c | tr -s ' ' | sort -t ' ' -k 2,2nr | cut -d ' ' -f 3 | head -n 3)

for v in $sites; do
	cnt2=$(egrep -c "$v.*HTTP/2\.0" "${1}")
	cnt1=$(egrep -c "$v.*HTTP/1\.[01]" "${1}")
	top5=$(mktemp)
	cat "${1}" | egrep "\ $v\ " | awk -F ' ' '{if ($9>302) {print $1} }' | sort | uniq -c | tr -s ' ' | sort -t ' ' -k 2,2nr | head -n 5 >> top5	
	
	echo "$v HTTP/2.0: $cnt2 non-HTTP/2.0: $cnt1"
	cat top5

	rm top5
done
