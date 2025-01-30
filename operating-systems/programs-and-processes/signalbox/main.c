#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define clear_term() fputs("\033c", stdout)

volatile sig_atomic_t PRINT_TERMINAL_SIZE_FLAG = 1;
volatile sig_atomic_t EXIT_GRACEFULLY_FLAG     = 0;

void print_terminal_dimensions() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  printf("%i x %i\n", w.ws_xpixel, w.ws_ypixel);
}

void handle_sigint(int signal) {
  if (signal == SIGINT) {
    EXIT_GRACEFULLY_FLAG = 1;
  }
}

void handle_sigwinch(int signal) {
  if (signal == SIGWINCH) {
    PRINT_TERMINAL_SIZE_FLAG = 1;
  }
}

int main() {
  signal(SIGINT, handle_sigint);
  signal(SIGWINCH, handle_sigwinch);

  for (;;) {
    if (PRINT_TERMINAL_SIZE_FLAG) {
      clear_term();
      print_terminal_dimensions();
      PRINT_TERMINAL_SIZE_FLAG = 0;
    }
    if (EXIT_GRACEFULLY_FLAG) {
      printf("\n");
      exit(EXIT_SUCCESS);
    }
  }
}
