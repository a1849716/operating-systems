/*********************************************************************
   Program  : miniShell                   Version    : 1.3
 --------------------------------------------------------------------
   skeleton code for linix/unix/minix command line interpreter
 --------------------------------------------------------------------
   File			: minishell.c
   Compiler/System	: gcc/linux

********************************************************************/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NV 20  /* max number of command tokens */
#define NL 100 /* input buffer size */
char line[NL]; /* command input buffer */

/*
        shell prompt
 */

void prompt(void) {
  // fprintf(stdout, "\n msh> ");
  fflush(stdout);
}

int main(int argk, char *argv[], char *envp[])
/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */

{
  int frkRtnVal; /* value returned by fork sys call */
  // int             wpid;		/* value returned by wait */
  char *v[NV];         /* array of pointers to command line tokens */
  char *sep = " \t\n"; /* command line token separators    */
  int i;               /* parse index */

  /* prompt for and process one command line at a time  */

  while (1) { /* do Forever */
    prompt();
    fgets(line, NL, stdin);
    fflush(stdin);

    if (feof(stdin)) { /* non-zero on EOF  */
      //fprintf(stderr, "EOF pid %d feof %d ferror %d\n", getpid(), feof(stdin),
              //ferror(stdin));
      exit(0);
    }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
      continue; /* to prompt */

    v[0] = strtok(line, sep);
    for (i = 1; i < NV; i++) {
      v[i] = strtok(NULL, sep);
      /*if(strcmp(v[i], "&" == 0)){
        //change to background process
      } else */if (v[i] == NULL) break;
    }
    /* assert i is number of tokens + 1 */

    // Handle "cd" command
    if (strcmp(v[0], "cd") == 0 && v[1] != NULL) {
      if (chdir(v[1]) != 0 || v[1] == NULL) {
        perror("cd failed L bozo");
      }
      continue;
    }

    /* fork a child process to exec the command in v[0] */
    switch (frkRtnVal = fork()) {
      case -1: /* fork returns error to parent process */
      {
        perror("fork failed L bozo");
        break;
      }
      case 0: /* code executed only by child process */
      {
        if(execvp(v[0], v) == -1){
          //printf("Process did not terminate correctly\n");
          exit(1);
        };
      }
      default: /* code executed only by parent process */
      {
        wait(0);
        // printf("%s done \n", v[0]);
        break;
      }
    } /* switch */
  } /* while */
} /* main */
