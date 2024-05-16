SHELL := /bin/bash

clear_cache:
	@sync
	@echo 3 | sudo tee /proc/sys/vm/drop_caches > /dev/null

original:
	gcc -g -O0 convert.c
	$(MAKE) clear_cache
	cat css/simple.css | ./a.out

extern:
	gcc -g -O0 convert.c process_token.s -DUSE_EXTERN
	$(MAKE) clear_cache
	cat css/simple.css | ./a.out

extern_refactored:
	gcc -g -O0 convert.c refactored_process_token.s -DUSE_EXTERN
	$(MAKE) clear_cache
	cat css/simple.css | ./a.out
