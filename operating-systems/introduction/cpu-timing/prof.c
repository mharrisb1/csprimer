#define _GNU_SOURCE

#include <sched.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>

#include "prof.h"

static struct timeval timeval_subtract(struct timeval end, struct timeval start) {
	struct timeval result;
	result.tv_sec = end.tv_sec - start.tv_sec;
	result.tv_usec = end.tv_usec - start.tv_usec;
	if (result.tv_usec < 0) {
		result.tv_usec += 1000000;
		result.tv_sec -= 1;
	}
	return result;
}

void profile_start(struct profile_times *t, char *msg, ...) {
	t->pid = getpid();
	t->cpu = sched_getcpu();

	getrusage(RUSAGE_SELF, &t->usage);
	gettimeofday(&t->start, NULL);

	va_list args;
	va_start(args, msg);

	printf("[pid %d, cpu %i] ", t->pid, t->cpu);
	vprintf(msg, args);

	va_end(args);
	
	printf("\n");
}

void profile_log(struct profile_times *t) {
	struct rusage usage;
	struct timeval real_end, real_diff, user_diff, sys_diff;
	
	getrusage(RUSAGE_SELF, &usage);
	gettimeofday(&real_end, NULL);

	real_diff = timeval_subtract(real_end, t->start);
	user_diff = timeval_subtract(usage.ru_utime, t->usage.ru_utime);
	sys_diff = timeval_subtract(usage.ru_stime, t->usage.ru_stime);

	printf("[pid %d, cpu %i] real: %ld.%03lds user: %ld.%03lds sys: %ld.%03lds\n",
		t->pid,
		t->cpu,
		real_diff.tv_sec, real_diff.tv_usec / 1000,
		user_diff.tv_sec, user_diff.tv_usec / 1000,
		sys_diff.tv_sec, sys_diff.tv_usec / 1000);
}

