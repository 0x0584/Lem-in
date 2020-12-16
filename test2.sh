#!/bin/bash

for map in big/*
do
	echo
	expected=`head -2 $map | egrep -o '[0-9]+' | sed -n '2p'`
	mine=`./lem-in < $map | grep '^L' | wc -l`
	#cat out.json
	val=$(expr $mine - $expected)
	echo -n $map " "
	if [ $val -gt 0 ]
	then
		echo -e "mine: \x1b[36m$mine\x1b[0m - expected: \x1b[36m$expected\x1b[0m = \x1b[31m$val\x1b[0m"
	elif [ $val -le 0 ]
	then
		echo -e "mine: \x1b[36m$mine\x1b[0m - expected: \x1b[36m$expected\x1b[0m = \x1b[32m$val\x1b[0m"
	fi
	# sleep 1
done
