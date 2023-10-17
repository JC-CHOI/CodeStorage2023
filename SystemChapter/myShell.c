#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// readcmd
void readcmd(char *);
// parsecmd
void parsecmd(char *);
// invokecmd
void invokecmd();

char **cmd_vector;
short cmd_count ;

#define INIT_CNT 10

int
main( int argc, char *argv[])
{
    char buf[BUFSIZ];

    cmd_vector = malloc( sizeof(char *) * INIT_CNT);
    while(1)
    {        
        fprintf(stdout,"$$ ");
        fflush(stdout);

        // 1. read input cmd
        readcmd(buf);
        // 2. parse argument in cmd
        parsecmd(buf);


        for( int i = 0 ; i < cmd_count ; i++)
            printf("%s\n",cmd_vector[i]);

        invokecmd();
    }
    free(cmd_vector);

}
void readcmd(char *buf)
{
    // exit , Ctrl+D
    if( fgets(buf, BUFSIZ, stdin) == NULL)
        exit(0);
}
void parsecmd(char *buf)
{
    char *p;
    p = buf;
    while(*p)
    {
        while(*p && isspace((int)*p)) // loop while blank
            *p++ = '\0';                
        
        if( *p )
            cmd_vector[cmd_count++] = p;        

        while( *p && !isspace((int)*p)) // loop while not blank
            p++;            
    }
}
void invokecmd()
{
    pid_t pid;

    pid = fork();
    if( pid < 0)
    {
        perror("fork : ");
        exit(1);
    }
    if( pid > 0) // parent
    {
        waitpid(pid, NULL, 0 );        
    }
    else
    {
        execvp(cmd_vector[0], cmd_vector);
        // error
        fprintf(stderr, "command not found %s\n", cmd_vector[0]);
        exit(1);
    }
}