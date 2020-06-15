#include "tty.h"

void lsh_loop(void)
{
  unsigned char *line;
  unsigned char *args;
  int status;

  do {
    printf("> ");
    line = (unsigned char *)lsh_read_line();
    free(line);
    free(args);
  } while (status);
}

#define LSH_RL_BUFSIZE 1024
char *lsh_read_line(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char buffer[128];// = ( char *)malloc(sizeof(char) * bufsize);
  int c;
    
  if (!buffer) {
    printf("lsh: allocation error\n");
  }

  while (1) {
    
    c = getchar();
    
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      //buffer = realloc(buffer, bufsize);
      if (!buffer) {
        printf("lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}


