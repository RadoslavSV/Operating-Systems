#!/bin/bash

#if [ "$USER" != "root" ]; then
#	exit 1
#fi

pesho=$(mktemp)
users=$(ps -eo user= | sort | uniq)
for v in $users; do
	ps -eo user=,rss= | egrep "^${v}" | awk 'BEGIN{sum=0} {sum=sum+$2} END{print $1,sum}' >> pesho
done
cat pesho

users2=$(cat pesho | awk -v var=${1} '{if ($2>var) {print $1}}')
for v in $users2; do
	maxPid=$(ps -eo user=,pid=,rss= | egrep "^${v}" | tr -s ' ' | sort -t ' ' -k 3,3nr | head -n 1 | cut -d ' ' -f 2)
	echo kill $maxPid
done

rm pesho
