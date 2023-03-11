#!/bin/bash

key="${2}"
value="${3}"

sed -r -i "s/^\s*$key\s*=.*$/# & # edited at $(date) by $USER\n$key = $value # added at $(date) by $USER/g" "${1}"

egrep -q "^\s*$key\s*=" "${1}"
if [ $? -ne 0 ]; then
	echo "$key = $value # added at $(date) by $USER" >> "${1}"
fi
