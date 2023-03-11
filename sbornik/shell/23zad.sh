#!/bin/bash

#if [ $USER != 'root' ]; then echo NO; exit 1; fi

pesho=$(mktemp)
homes=$(cat /etc/passwd | cut -d ':' -f 6 | egrep -v "/$USER")
for v in $homes; do
	find $v -maxdepth 1 -type f -printf "%T@ %u %P\n" 2>/dev/null >> pesho
done

cat pesho | sort -t ' ' -k 1,1nr | cut -d ' ' -f 2,3 | head -n 1 

rm pesho
