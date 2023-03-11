#!/bin/bash

if [ $USER != "root" ]; then
	echo "Script to execute as root"
	exit 1
fi

users=$(ps -eo user= | sort | uniq)

for v in $users; do
	ps -eo user=,rss= | egrep "^$v" | awk -F ' ' 'BEGIN{cnt=0;sum=0;} {cnt=cnt+1;sum=sum+$2;} END{printf("User %s has %s processes with a total of %s RSS\n"),$1,cnt,sum}'
done

for v in $users; do
	avg=$(ps -eo user=,rss= | egrep "^$v" | awk -F ' ' 'BEGIN{cnt=0;sum=0;} {cnt=cnt+1;sum=sum+$2;} END{sum=(sum/cnt)*2; printf("%i\n",sum)}' 2>/dev/null)
	pair=$(ps -eo user=,rss=,pid= | egrep "^$v" | tr -s ' ' | sort -t ' ' -k 2,2nr | head -n 1 | cut -d ' ' -f 2,3)
	maxRss=$(echo $pair | cut -d ' ' -f 1)
	rssPid=$(echo $pair | cut -d ' ' -f 2)
	
	if [ "$maxRss" -gt "$avg" 2>/dev/null ]; then	
		echo kill $rssPid
	fi
done
