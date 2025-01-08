#ifndef PROF_H
#define PROF_H

#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>

struct profile_times {
	pid_t pid;
	int cpu;
	struct timeval start;
	struct rusage usage;
};

void profile_start(struct profile_times *t, char *msg, ...);

void profile_log(struct profile_times *t);

#endif // PROF_H
