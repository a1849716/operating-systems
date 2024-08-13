#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigint(){
    printf("Yeah!\n");
}

void sighup(){
    printf("Ouch!\n");
}

int main(int argc, char *argv[])
{
    int a = atoi(argv[1]);
    signal(SIGINT, sigint);
    signal(SIGHUP, sighup);
    for(int i = 0; i < a; i++){
        printf("%d\n", i*2);
        sleep(5);
    }
    return 0;
}