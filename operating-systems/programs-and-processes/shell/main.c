#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXARG 256
#define MAXBUF 4096

static const char* const PROMPT = "> ";
static const char* const DELIM  = " \t\n";

static int  tokenize(char *input, char *argv[], size_t *argc);
static void print_help();

int main() {
	char buf[MAXBUF];

	for (;;) {
		printf(PROMPT);
		
		if (fgets(buf, MAXBUF, stdin) == NULL) {
			if (feof(stdin) != 0) exit(0);
			if (ferror(stdin) != 0) {
				perror("fgets error");
				exit(1);
			}
		}

		size_t argc = 0;
		char *argv[MAXARG];

		if (tokenize(buf, argv, &argc) != 0) exit(1);
		if (argc == 0) 			     continue;
		if (strcmp(argv[0], "quit") == 0)    exit(0);
		if (strcmp(argv[0], "help") == 0)    print_help();

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

static void print_help() {
	printf("Builtins are `quit` and `help`\n");
}
