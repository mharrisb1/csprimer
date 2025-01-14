#include <stdio.h>

int recurse(int depth) {
	printf("frame: %d (%p)\n", depth, &depth);
	return recurse(++depth);
}

int main(int argc, char **argv) {
	recurse(0);
}
