#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "readline.h"

#define MAXBUF 256

static struct termios orig;
static void           setup();
static void           teardown();

char *readline(const char *prompt) {
  setup();

  char  *buf = malloc(MAXBUF);
  size_t len = MAXBUF;
  size_t pos = 0;

  if (!buf) {
    teardown();
    return NULL;
  }

  write(STDOUT_FILENO, prompt, strlen(prompt));

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    teardown();
    break;
  }

  buf[pos] = '\0';
  teardown();

  return buf;
}

static void handle_interrupts(int sig) {
  (void)sig;
  teardown();
  exit(EXIT_FAILURE);
}

static void setup() {
  signal(SIGINT, handle_interrupts);
  signal(SIGTERM, handle_interrupts);

  struct termios raw;
  tcgetattr(STDIN_FILENO, &orig);
  atexit(teardown);

  raw = orig;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN]  = 1;
  raw.c_cc[VTIME] = 0;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    perror("Failed to enable raw mode");
    exit(EXIT_FAILURE);
  }
}

static void teardown() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig) == -1) {
    perror("Failed to disable raw mode");
  }
}
