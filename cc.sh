#!/bin/sh

gcc -Wall -Wextra -g -o lem_out src/*.c -Ilibft -Llibft -lft -I. &&
./lem_out
