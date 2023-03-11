#!/bin/bash

mkdir -p "${2}"
mkdir -p "${3}"
touch "${4}"


unzip "${1}" -d "${2}" &>/dev/null


find "${2}" -mindepth 1 -maxdepth 1 -printf "%p\n" 2>/dev/null | while read n; do
	mv $n $(dirname $n)/$(echo $n | sed -r 's%^.+/([0-9a-zA-Z]+)-.+$%\1%g')
done


find "${2}" -mindepth 2 2>/dev/null | egrep -v '\.tar\.xz$' | while read N; do
	mkdir -p ${3}/$(basename $(dirname $N))
	cp $N ${3}/$(basename $(dirname $N))/$(basename $N)
done


find "${2}" -mindepth 1 -maxdepth 1 2>/dev/null | while read L; do
	if [ $(find "$L" -mindepth 1 2>/dev/null | wc -l) -gt 1 ]; then
		mkdir -p ${3}/$(basename $(dirname $N))
		cp $N ${3}/$(basename $(dirname $N))/$(basename $N)
	fi
done


find "${2}" -mindepth 1 -maxdepth 1 2>/dev/null | while read P; do
	echo "$(basename ${P})" >> "${4}"
done
sort -n -o "${4}" "${4}"

pesho=$(mktemp)
find "${2}" -mindepth 2 -maxdepth 2 -printf "%P\n" 2>/dev/null | while read u; do
	echo $u >> pesho
done
sort -n -o pesho pesho
goshu=$(mktemp)
cat pesho | cut -d '/' -f 2 >> goshu

paste "${4}" goshu | while read a b; do
	fn=$(echo $b | cut -d '.' -f 1)
	if [ "${a}" == "${fn}" ] && [[ "${b}" =~ \.tar\.xz ]]; then
		sed -r -i "s/^${a}.*$/& 0/g" "${4}"
	else
		sed -r -i "s/^${a}.*$/& 1/g" "${4}"	
	fi
done

rm pesho goshu


find "${2}" -mindepth 2 -maxdepth 2 2>/dev/null | while read r; do
    FN=$(basename $(dirname "${r}"))
	ext=$(file "${r}")
	if [[ $ext =~ 'XZ compressed data' ]]; then
		sed -r -i "s/^${FN}.*$/& 0/g" "${4}"
	else
		sed -r -i "s/^${FN}.*$/& 1/g" "${4}"
	fi
done


find "${2}" -mindepth 2 -maxdepth 2 2>/dev/null | while read w; do
	dN=$(dirname ${w})
	tar -xf "${w}" -C "${dN}" &>/dev/null
	if [ $? -gt 0 ]; then
		cp -r ${dN} ${3}
	fi
done


find "${2}" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | while read p; do
	No=$(basename ${p}) 
	Fl=$(wc -l < <(find "${p}" -mindepth 1 -maxdepth 1 -type d 2>/dev/null))
	if [ $Fl -gt 0 ]; then
		sed -r -i "s/^${No}.*$/& 0/g" "${4}"
	else
		sed -r -i "s/^${No}.*$/& 1 1/g" "${4}"
	fi
done


find "${2}" -mindepth 2 -maxdepth 2 -type d  2>/dev/null | while read y; do
	fN=$(basename $(dirname $y)) 
	dN=$(basename $y)
	if [ "${fN}" == "${dN}" ]; then
		sed -r -i "s/^${fN}.*$/& 0/g" "${4}"
	else
		sed -r -i "s/^${fN}.*$/& 1/g" "${4}"
	fi
done


egrep '\.(tar|zip)' <(find "${2}" 2>/dev/null | while read s; do
	echo $s
done) | while read S; do
	rm -r $S
done


find "${2}" -mindepth 2 -maxdepth 2 -type d 2>/dev/null | while read U; do
	if [[ $(basename ${U}) =~ ^[0-9] ]]; then
		cp -r "${U}" $(dirname $(dirname $U))
	fi
done


find "${2}" -mindepth 2 -type d 2>/dev/null | while read T; do
	rm -r $T &>/dev/null
done



