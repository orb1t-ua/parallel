#!/bin/bash
for i in {1..10}; do
	./p3.out 1 5000 >> 1.txt
done
for i in {1..10}; do
	./p3.out 4 5000 >> 4.txt
done


