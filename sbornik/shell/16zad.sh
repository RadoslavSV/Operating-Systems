#!/bin/bash

#if [ $USER != "root" ]; then
#	echo Script must be executed as root!
#	exit 1
#fi

pesho=$(mktemp)
ps -eo pid=,user=,rss= | sort -k 2,2 -k 3,3n > pesho
A=$(cat pesho | awk '{print $2}' | uniq)

for usr in $A; do
	sumRss=$(cat pesho | egrep $usr | awk 'BEGIN{n=0;} {n+=$3;} END{print n}')
	maxPid=$(cat pesho | egrep $usr | awk 'BEGIN{m=$1;} {m=$1;} END{print m}')
	
	echo $usr $sumRss
	if [ $sumRss -gt ${1} ]; then
		echo kill $maxPid   # SHOULD ACTUALLY KILL THE PROCESS
	fi
done

rm pesho
