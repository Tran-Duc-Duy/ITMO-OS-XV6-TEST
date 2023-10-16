#include "types.h"
#include "stat.h"
#include "user.h"

const char *parent_message = "Hello from parent";
const char *child_message = "Hello from child";
int main(int argc, char **argv)
{
  int p2c_pipefd[2];
  int c2p_pipefd[2];

  if (pipe(p2c_pipefd) < 0 || pipe(c2p_pipefd) < 0)
  {
    printf("Failed to initialize pipe\n");
    exit(1);
  }

  int pid = fork();

  if (pid == 0)
  {
    close(p2c_pipefd[1]);
    close(c2p_pipefd[0]);
    char *buf = malloc(sizeof(char) * 100);
    int size = 0;
    for (; read(p2c_pipefd[0], buf + size, 1) > 0; size++)
      ;
    buf[size] = 0;
    printf("%d: got %s\n", getpid(), buf);
    close(p2c_pipefd[0]);
    write(c2p_pipefd[1], child_message, strlen(child_message));
    close(c2p_pipefd[1]);

    free(buf);
  }
  else
  {
    close(p2c_pipefd[0]);
    close(c2p_pipefd[1]);
    write(p2c_pipefd[1], parent_message, strlen(parent_message));
    close(p2c_pipefd[1]);
    char *buf = malloc(sizeof(char) * 100);
    int size = 0;
    for (; read(c2p_pipefd[0], buf + size, 1) > 0; size++)
      ;
    buf[size] = 0;
    printf("%d: got %s\n", getpid(), buf);
    close(c2p_pipefd[0]);
    free(buf);
  }

  exit(0);
}
