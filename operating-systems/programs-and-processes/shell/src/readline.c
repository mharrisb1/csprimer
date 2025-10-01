#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "readline.h"

#define NUL   0x00
#define ETX   0x03
#define EOT   0x04
#define BS    0x08
#define LF    0x0A
#define CR    0x0D
#define ESC   0x1B
#define NAK   0x15
#define SPACE 0x20
#define DEL   0x7F

#define CRLF ((char[]){CR, LF})
#define CUR  ((char[]){BS})

#define next_line()        write(STDOUT_FILENO, CRLF, 2)
#define move_cursor_left() write(STDOUT_FILENO, CUR, 1)

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
static int            setup();
static int            teardown();
static int            read_keypress(const int fd, char seq[4], ssize_t *n);

static void handle_eof();
static void handle_key(const int key, char *buf, const size_t len,
                       const size_t cur);
static void handle_backspace(char *buf, const size_t len, const size_t cur);
static void handle_arrow_left();
static void handle_arrow_right(const char *buf, const size_t cur);

int readline(const char *prompt, char *buf) {
  if (setup() != 0) { return -1; }

  size_t  len = 0, cur = 0;
  ssize_t n;
  char    seq[4];

  write(STDOUT_FILENO, prompt, strlen(prompt));

  int key;
  while ((key = read_keypress(STDIN_FILENO, seq, &n)) != CR) {
    memset(seq, 0, sizeof(seq)); // reset
    if (key == EOF) {
      handle_eof();
      teardown();
      return 1;
    } else if (key >= SPACE && key < DEL) {
      handle_key(key, buf, len, cur);
      len++;
      cur++;
    } else if ((key == BS || key == DEL) && cur > 0) {
      handle_backspace(buf, len, cur);
      len--;
      cur--;
    } else if (key == XTERM_LEFT && cur > 0) {
      handle_arrow_left();
      cur--;
    } else if (key == XTERM_RIGHT && cur < len) {
      handle_arrow_right(buf, cur);
      cur++;
    }
  }

  next_line();

  buf[len] = '\0';
  return teardown();
}

static int setup() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &orig);

  raw = orig;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN]  = 1;
  raw.c_cc[VTIME] = 0;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    perror("Failed to enable raw mode");
    return -1;
  }
  return 0;
}

static int teardown() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig) == -1) {
    perror("Failed to disable raw mode");
    return -1;
  }
  return 0;
}

static int read_keypress(const int fd, char seq[4], ssize_t *n) {
  *n = read(fd, seq, 4);

  if (*n < 1 || seq[0] == EOT || seq[0] == ETX) return EOF;
  if (seq[0] != ESC || *n == 1) return seq[0];

  // ANSI Escape Codes (https://en.wikipedia.org/wiki/ANSI_escape_code)
  if (seq[1] != '[' || *n == 2) return seq[0]; // Non-CSI

  // Terminal input sequences
  // (https://en.wikipedia.org/wiki/ANSI_escape_code#Terminal_input_sequences)

  // Assume xterm (subset only)
  if (*n == 3) {
    switch (seq[2]) {
      case 'A': return XTERM_UP;
      case 'B': return XTERM_DOWN;
      case 'C': return XTERM_RIGHT;
      case 'D': return XTERM_LEFT;
      case 'F': return XTERM_END;
      case 'H': return XTERM_HOME;
    }
  }
  // Assume vt (subset only)
  else if (seq[3] == '~') {
    switch (seq[2]) {
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

static void handle_eof() { next_line(); }

static void handle_key(const int key, char *buf, const size_t len,
                       const size_t cur) {
  memmove(buf + cur + 1, buf + cur, len - cur);
  buf[cur] = key;

  size_t head = len + 1 - cur;
  write(STDOUT_FILENO, buf + cur, head);
  for (size_t i = 0; i < head - 1; i++) {
    move_cursor_left();
  }
}

static void handle_backspace(char *buf, const size_t len, const size_t cur) {
  memmove(buf + cur - 1, buf + cur, len - cur);
  move_cursor_left();
  size_t tail = len - cur;
  write(STDOUT_FILENO, buf + cur - 1, tail);
  write(STDOUT_FILENO, " ", 1);
  for (size_t i = 0; i < tail + 1; i++) {
    move_cursor_left();
  }
}

static void handle_arrow_left() { move_cursor_left(); }

static void handle_arrow_right(const char *buf, const size_t cur) {
  write(STDOUT_FILENO, &buf[cur], 1);
}
