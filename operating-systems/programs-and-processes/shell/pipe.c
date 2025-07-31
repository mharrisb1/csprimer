#include <unistd.h>

#define R_END 0
#define W_END 1

int main() {
	// GOAL: implement ls | wc
	int fds[2]; // file descriptors that will be opened by `pipe`
	pipe(fds);
	if (fork() == 0) {
		// child
		dup2(fds[W_END], STDOUT_FILENO); // duplicate the object descriptor at fds[1] at index reserved for stdout  
		close(fds[R_END]);		 // no need to keep around
		close(fds[W_END]);		 // no need to keep around
		execlp("ls", "ls", NULL);	 // ls will write to object descriptor at index that is normally stdout but is now a copy of the object descriptor at fds[1] 
	}
	// parent
	dup2(fds[R_END], STDIN_FILENO); // duplicate the object descriptor at fds[0] at index reserved for stdin 
	close(fds[R_END]);	    	// no need to keep around
	close(fds[W_END]);  	    	// no need to keep around
	execlp("wc", "wc", NULL);   	// wc will read from object descriptor at index that is normally stdin but is now a copy of the object descriptor at fds[0]
}

