#!/bin/bash

gcc -c main.c
gcc -c f.S
gcc -o a.out main.o f.o

./a.out
