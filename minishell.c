/*********************************************************************
   Program  : miniShell                   Version    : 1.3
 --------------------------------------------------------------------
   skeleton code for linix/unix/minix command line interpreter
 --------------------------------------------------------------------
   File			: minishell.c
   Compiler/System	: gcc/linux

********************************************************************/

#include <signal.h>
#include <stdbool.h>
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

//struct for the background process
struct background_struct {
  int process_id;
  char process_name[NL];
  bool execution_stat;
};

//counter for background process 
int background_counter = 0;
struct background_struct lmao[20];

//print function to check status of background processes
void print_background_thing(){
  int process_id_print;
  for(int k = 0; k < background_counter; k++){
    if(!lmao[k].execution_stat){
      process_id_print = waitpid(lmao[k].process_id, NULL, WNOHANG);
      if(process_id_print > 0){
        lmao[k].execution_stat = true;
        printf("[%d]+ Done %s\n", k+1, lmao[k].process_name);
      }
    }
  }
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
    
    //calling print function after each input
    print_background_thing();

    if (feof(stdin)) { /* non-zero on EOF  */
      // fprintf(stderr, "EOF pid %d feof %d ferror %d\n", getpid(),
      // feof(stdin), ferror(stdin));
      exit(0);
    }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
      continue; /* to prompt */

    v[0] = strtok(line, sep);
    for (i = 1; i < NV; i++) {
      v[i] = strtok(NULL, sep);
      if (v[i] == NULL) break;
    }
    /* assert i is number of tokens + 1 */

    // Handle "cd" command
    if (strcmp(v[0], "cd") == 0 && v[1] != NULL) {
      if (chdir(v[1]) != 0 || v[1] == NULL) {
        perror("cd failed L bozo");
      }
      continue;
    }
    // detecting "&"
    bool background = false;
    if (strcmp(v[i - 1], "&") == 0) {
      background = true;
      v[i - 1] = NULL;
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
        execvp(v[0], v);
        perror("execvp failed L bozo");
        exit(EXIT_FAILURE);
      }
      default: /* code executed only by parent process */
      { 
        //if the child process is a background process
        if (background) {
          //updating values
          lmao[background_counter].execution_stat = false;
          lmao[background_counter].process_id = frkRtnVal;
          strcpy(lmao[background_counter].process_name, v[0]);
          strcat(lmao[background_counter].process_name, " ");
          strcat(lmao[background_counter].process_name, v[1]);
          printf("[%d] %d\n", background_counter+1, frkRtnVal);
          background_counter++;
        } else {
          wait(0);
        }
        // printf("%s done \n", v[0]);
        break;
      }
    } /* switch */
  } /* while */
} /* main */
