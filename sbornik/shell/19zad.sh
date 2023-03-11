#!/bin/bash

cnt1=$(cat "${1}" | egrep -c " ${1} ")
cnt2=$(cat "${2}" | egrep -c " ${2} ")

if [ $cnt1 -ge $cnt2 ]; then
	file="${1}"
else
	file="${2}"
fi

cat $file | sed -r "s/^.*${file} - //g" | sort > "${file}.songs"
