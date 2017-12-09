#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
extern char **environ;
extern int errno;
int main (int argc, char **argv) {
  uid_t uid, euid;
  uid=getuid();
  euid=geteuid();
  printf ("UID %d - EUID %d\n", uid, euid);
  setreuid (euid, euid);
  uid=getuid();
  euid=geteuid();
  printf ("UID %d - EUID %d\n", uid, euid);
  execve("/bin/bash", NULL, environ);
  printf ("Error : %d\n", errno);
  return errno;
}
