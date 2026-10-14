#!/usr/bin/env bash

make

echo $(($1 + $2)) > answer.txt
echo $(($1 - $2)) >> answer.txt

./w3 $1 $2 > output.txt

diff answer.txt output.txt > result.txt

make clean
