#!/bin/bash

for i in $(seq 1 10)
do	
	./client -d $i &
done
