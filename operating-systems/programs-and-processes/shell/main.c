#include <stdio.h>
#include <string.h>

#define MAXARG 256
#define MAXBUF 4096

static const char* const PROMPT = "> ";
static const char* const DELIM  = " \t\n";

static int tokenize(char *input, char* argv[], size_t* argc);

int main() {
	char buf[MAXBUF];

	for (;;) {
		printf(PROMPT);
		
		if (fgets(buf, MAXBUF, stdin) == NULL) {
			if (feof(stdin) != 0) return 0;
			if (ferror(stdin) != 0) {
				perror("fgets error");
				return 1;
			}
		}

		size_t argc = 0;
		char *argv[MAXARG];

		if (tokenize(buf, argv, &argc) != 0) return 1;

		printf("argc = %zu\n", argc);
		for (size_t i = 0; i < argc; i++) {
			printf("argv[%zu] = '%s'\n", i, argv[i]);
		}
	}
	
	return 0;
}

static int tokenize(char *input, char *argv[], size_t *argc) {
	*argc = 0;

	char *rest = input;
	char *tok;

	while ((tok = strsep(&rest, DELIM)) != NULL) {
		if (*tok == '\0') continue;
		if (*argc < MAXARG - 1) {
			argv[(*argc)++] = tok;
		} else {
			fprintf(stderr, "Too many arguments\n");
			return -1;
		}
	}
	return 0;
}
