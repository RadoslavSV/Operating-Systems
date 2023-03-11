#!/bin/bash

#if [ $USER != 'root' ]; then
#	echo Script must be executed as root!
#	exit 1
#fi

foo="${1}"
users=$(ps -eo user= | sort | uniq)

echo "Users with more processes than $foo:"

for v in $users; do
	cnt_v=$(ps -eo user= | egrep -c "^$v")
	cnt_foo=$(ps -eo user= | egrep -c "^$foo")
	
	if [ $cnt_v -gt $cnt_foo ]; then
		echo $v
	fi
done

echo -e "\nAverage time for all processes of each user:"

for v in $users; do
	avg_v=$(ps -eo user=,time= | egrep "^$v " | awk '{print $2}' | awk -F ':' '{sec+=( $1*3600 + $2*60 + $3 ); cnt_v++;}; END{if (cnt_v != 0) {print sec/cnt_v} else {print 0} }')
	echo $v $avg_v
done

echo -e "\nKilling processes of $foo that have been running for more than doubled the average time for $foo:"

PS=$(mktemp)
PS2=$(mktemp)

ps -eo user=,pid=,time= | egrep "^$foo" > PS 
cat PS | awk '{print $2,$3}' > PS2

pids_foo=$(cat PS | awk '{print $2}')
avg_foo=$(cat PS2 | awk '{print $2}' | awk -F ':' '{sec+=( $1*3600 + $2*60 + $3 ); cnt_v++;}; END{if (cnt_v != 0) {print sec/cnt_v} else {print 0} }')
avg_foo=$(echo "scale=2; $avg_foo*2" | bc)

for v in $pids_foo; do
	sec_v=$(cat PS2 | egrep "^$v" | awk '{print $2}' | awk -F ':' '{sec=( $1*3600 + $2*60 + $3 )}; {print sec}')
	pid=$(echo $v $sec_v $avg_foo | awk '{if ($2 > $3) {print $1} else {printf("err")} }')
	
	if [ ! $pid == 'err' ]; then echo kill $pid; fi
done

rm PS PS2
