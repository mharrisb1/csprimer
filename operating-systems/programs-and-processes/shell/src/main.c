#define _POSIX_SOURCE
#define _DEFAULT_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "readline.h"

#define MAXCMD 8
#define MAXARG 256
#define MAXBUF 2048

static const char *const PROMPT = "> ";
static const char *const DELIM  = " \t\n";

static void handle_sigint(int sig);
static int  tokenize(char *input, char *cmds[MAXCMD][MAXARG], int *cmdi,
                     int *argi);

volatile pid_t childpid = 0;

int main() {
  char  buf[MAXBUF];
  int   argi, cmdi;
  char *cmds[MAXCMD][MAXARG];

  signal(SIGINT, handle_sigint);

  while ((readline(PROMPT, buf)) == 0) {
    argi = 0;
    cmdi = 0;
    if (tokenize(buf, cmds, &cmdi, &argi) != 0) exit(EXIT_FAILURE);

    // eval
    if (cmds[0][0] == NULL) continue;
    if (strcmp(cmds[0][0], "quit") == 0) exit(EXIT_SUCCESS);
    if (strcmp(cmds[0][0], "help") == 0) printf("Type `quit` to exit\n");

    // single command
    if (cmdi == 0) {

      if ((childpid = fork()) < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
      }

      // child process
      if (childpid == 0) {
        if (execvp(cmds[cmdi][0], cmds[cmdi]) < 0) {
          perror("exec error");
          exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
      }

      // parent
      int status;
      waitpid(childpid, &status, 0);
    }
    buf[0] = '\0';
  }

  return EXIT_SUCCESS;
}

static void handle_sigint(int sig) {
  if (!childpid) return;
  if (kill(childpid, sig) < 0) perror("SIGINT error");
}

#ifdef DEBUG
static void debug_command_table(char *cmds[MAXCMD][MAXARG], int cmdi, int argi) {
  for (int i = 0; i <= cmdi; i++) {
    printf("Command[%i] {\n", i);
    for (int j = 0; j <= argi; j++) {
      if (cmds[i][j] == NULL) continue;
      printf("  Arg[%i] = %s,\n", j, cmds[i][j]);
    }
    printf("}\n");
  }
}
#endif

static int tokenize(char *input, char *cmds[MAXCMD][MAXARG], int *cmdi,
                    int *argi) {
  char *tok, *rest = input;

  while ((tok = strsep(&rest, DELIM)) != NULL) {
    if (*tok == '\0') continue;
    if (*tok == '|') {
      cmds[(*cmdi)++][(*argi)] = NULL;
      *argi                    = 0;
      continue;
    }
    if ((*cmdi < MAXCMD - 1) && (*argi < MAXARG - 1)) {
      cmds[*cmdi][(*argi)++] = tok;
    }
  }
  cmds[(*cmdi)][(*argi)] = NULL;
#ifdef DEBUG
  debug_command_table(cmds, *cmdi, *argi);
#endif
  return 0;
}
