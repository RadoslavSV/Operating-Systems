#!/bin/bash

#if [ $USER != 'root' ]; then
#	echo Script must be executed as root!
#	exit 1
#fi

IFS=':'
cat /etc/passwd | cut -d ':' -f 1,6 | while read a b; do
	if [ "${a}" != "$(basename "${b}")" ] || [[ ! "$(stat -c %A "${b}" 2>/dev/null)" =~ ^..w.*$ ]]; then
		echo "${a}"
	fi
done
