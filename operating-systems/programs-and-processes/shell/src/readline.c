#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "readline.h"

#define MAXBUF 256

#define NUL 0x00
#define ETX 0x03
#define EOT 0x04
#define LF  0x0A
#define CR  0x0D
#define ESC 0x1B
#define NAK 0x15
#define DEL 0x7F

#define CRLF ((char[]){CR, LF})

enum vt_sequence_t {
  VT_HOME = 1000,
  VT_INSERT,
  VT_DELETE,
  VT_END,
  VT_PGUP,
  VT_PGDN
};

enum xterm_sequence_t {
  XTERM_UP = 2000,
  XTERM_DOWN,
  XTERM_RIGHT,
  XTERM_LEFT,
  XTERM_END,
  XTERM_HOME
};

static struct termios orig;
static void           setup();
static void           teardown();
static int            read_keypress(const int fd, char buf[4], ssize_t *n);

char *readline(const char *prompt) {
  setup();

  char   *buf = malloc(MAXBUF);
  size_t  len = MAXBUF, pos = 0;
  ssize_t n;
  char    seq[4];

  if (!buf) {
    teardown();
    perror("Failed to allocate buffer");
    return NULL;
  }

  write(STDOUT_FILENO, prompt, strlen(prompt));

  int key;
  while ((key = read_keypress(STDIN_FILENO, seq, &n)) != CR) {}

  write(STDOUT_FILENO, CRLF, 2);

  buf      = realloc(buf, len + 1);
  buf[len] = '\0';
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

static int read_keypress(const int fd, char buf[4], ssize_t *n) {
  *n = read(fd, buf, 4);

  if (*n < 1 || buf[0] == EOT) return EOF;
  if (buf[0] != ESC || *n == 1) return buf[0];

  // ANSI Escape Codes (https://en.wikipedia.org/wiki/ANSI_escape_code)
  if (buf[1] != '[' || *n == 2) return buf[0]; // Non-CSI

  // Terminal input sequences
  // (https://en.wikipedia.org/wiki/ANSI_escape_code#Terminal_input_sequences)

  // Assume xterm (subset only)
  if (*n == 3) {
    switch (buf[2]) {
      case 'A': return XTERM_UP;
      case 'B': return XTERM_DOWN;
      case 'C': return XTERM_RIGHT;
      case 'D': return XTERM_LEFT;
      case 'F': return XTERM_END;
      case 'H': return XTERM_HOME;
    }
  }
  // Assume vt (subset only)
  else if (buf[3] == '~') {
    switch (buf[2]) {
      case '1': return VT_HOME;
      case '2': return VT_INSERT;
      case '3': return VT_DELETE;
      case '4': return VT_END;
      case '5': return VT_PGUP;
      case '6': return VT_PGDN;
      case '7': return VT_HOME;
      case '8': return VT_END;
    }
  }

  // default
  return ESC;
}
