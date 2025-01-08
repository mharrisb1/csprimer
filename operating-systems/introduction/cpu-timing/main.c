#include <stdlib.h>
#include <unistd.h>

#include "prof.h"

#define NUM_MULS    10000000
#define NUM_MALLOCS 100000
#define MALLOC_SIZE 1000
#define SLEEP_SEC   10

int main() {
	struct profile_times t;	
	
	float x = 1.0;
	profile_start(&t, "%i muls", NUM_MULS);
	for (int i = 0; i < NUM_MULS; i++) {
		x *= 1.1;
	}
	profile_log(&t);

	void *p;
	profile_start(&t, "%i mallocs", NUM_MALLOCS);
	for (int i = 0; i < NUM_MALLOCS; i++) {
		p = malloc(MALLOC_SIZE);
	}
	profile_log(&t);

	profile_start(&t, "sleep %i seconds", SLEEP_SEC);
	sleep(SLEEP_SEC);
	profile_log(&t);
}
