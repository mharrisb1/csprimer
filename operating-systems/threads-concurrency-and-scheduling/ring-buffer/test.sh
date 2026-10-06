#!/bin/bash

gcc -g -O0 $VOY_EVENT_PATH
valgrind --leak-check=full --track-origins=yes -s ./a.out
