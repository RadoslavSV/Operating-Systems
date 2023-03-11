#!/bin/bash

cat "${1}" | sed -r 's/^[0-9]{4} г. - //g' | awk '{printf("%s. %s\n",NR,$0)}' | sort -t '.' -k 2,2
