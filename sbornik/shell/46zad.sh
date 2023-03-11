#!/bin/bash

egrep -q ",${2}," prefix.csv
if [ $? -ne 0 ]; then
	echo "No such prefix symbol"
	exit 1
fi
egrep -q ",${3}," base.csv
if [ $? -ne 0 ]; then
	echo "No such unit symbol"
	exit 1
fi

mult=$(cat prefix.csv | egrep ",${2}," | cut -d ',' -f 3)
num=$(echo "${1}*$mult" | bc)
three=$(cat base.csv | egrep ",${3}," | awk -F ',' '{printf("(%s, %s)\n",$3,$1)}')

echo "$num" "${3}" "$three"
