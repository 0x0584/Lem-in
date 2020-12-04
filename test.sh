#!/bin/bash

mkdir -p big
declare -a maps=("flow-one" "flow-ten" "flow-thousand" "big" "big-superposition")
for map in "${maps[@]}"
do
	for i in {00..100}
	do
		echo
		./generator "--$map" > big/$map-$i
		expected=`head -2 big/$map-$i | egrep -o '[0-9]+' | sed -n '2p'`
		mine=`./lem-in < big/$map-$i | grep '^L' | wc -l`
		val=$(expr $mine - $expected)
		echo -n $map-$i " "
		if [ $val -gt 0 ]
		then
			echo -e "$mine - $expected = \x1b[41m$val\x1b[0m"
		elif [ $val -le 0 ]
		then
			echo -e "\x1b[32m$val\x1b[0m"
		fi
	done
done
