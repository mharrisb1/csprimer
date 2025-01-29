#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define clear_term() fputs("\033c", stdout)

static int rows, cols;

void get_terminal_size(int *rows, int *cols) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  printf("%i x %i\n", w.ws_xpixel, w.ws_ypixel);
}

void handle_sigint(int signal) {
  if (signal == SIGINT) {
    printf("\n");
    exit(EXIT_SUCCESS);
  }
}

void handle_sigwinch(int signal) {
  if (signal == SIGWINCH) {
    clear_term();
    get_terminal_size(&rows, &cols);
  }
}

int main() {
  signal(SIGINT, handle_sigint);
  signal(SIGWINCH, handle_sigwinch);

  get_terminal_size(&rows, &cols);

  for (;;) {
  }
}
