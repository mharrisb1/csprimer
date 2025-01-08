#include <stdio.h>

int main() {
	for (int i = 0; i < 100; i++) {
		printf("%i", i);
		#ifdef NOBUF
		fflush(stdout);
		#endif
	}
}
