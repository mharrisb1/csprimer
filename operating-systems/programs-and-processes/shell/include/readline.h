#ifndef READLINE_H
#define READLINE_H

/**
 * Provides a simple line editor and returns the content when newline character
 * is received.
 *
 * @param prompt Line prompt
 * @return Heap-allocated string of the last state of the line buffer
 *
 * @note It is the caller's responsibility to free the returned string
 */
char *readline(const char *prompt);

#endif // READLINE_H
