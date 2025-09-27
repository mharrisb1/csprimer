#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXARG 256
#define MAXBUF 4096

static const char *const PROMPT = "> ";
static const char *const DELIM  = " \t\n";

static void print_help();
static void handle_sigint(int sig);
static int  tokenize(char *input, char *argv[], size_t *argc);

volatile pid_t childpid = 0;

int main() {
  char   buf[MAXBUF];
  size_t argc;
  char  *argv[MAXARG];

  signal(SIGINT, handle_sigint);

  for (;;) {
    printf(PROMPT);

    if (fgets(buf, MAXBUF, stdin) == NULL) {
      if (feof(stdin) != 0) exit(0);
      if (ferror(stdin) != 0) {
        perror("fgets error");
        exit(EXIT_FAILURE);
      }
    }

    // tokenization
    if (tokenize(buf, argv, &argc) != 0) exit(EXIT_FAILURE);

    // eval
    if (argc == 0) continue;
    if (strcmp(argv[0], "quit") == 0) exit(EXIT_SUCCESS);
    if (strcmp(argv[0], "help") == 0) {
      print_help();
      continue;
    }

    if ((childpid = fork()) < 0) {
      perror("fork error");
      exit(EXIT_FAILURE);
    }

    // child process
    if (childpid == 0) {
      if (execvp(argv[0], argv) < 0) {
        perror("exec error");
        exit(EXIT_FAILURE);
      }
      exit(EXIT_FAILURE);
    }

    // parent
    int status;
    waitpid(childpid, &status, 0);
  }

  return EXIT_SUCCESS;
}

static void print_help() { printf("Builtins are `quit` and `help`\n"); }

static void handle_sigint(int sig) {
  if (!childpid) return;
  if (kill(childpid, sig) < 0) perror("SIGINT error");
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
  argv[*argc] = NULL;
  return 0;
}
