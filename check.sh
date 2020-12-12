#!/usr/bin/bash

declare -a farms=(
	"crossmap"
	"map1"
	"map2"
	"map2_small.bak"
	"map5"
	"baaar"
	"foomap.bak"
)

make all
for f in "${farms[@]}"
do
	echo $f
	sleep 1
	./lem-in < $f
	echo "------------"
	sleep 1
done
