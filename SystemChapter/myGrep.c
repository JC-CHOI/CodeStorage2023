// 'grep' command with -i , -v options

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <unistd.h>

static void do_grep(regex_t *pat, FILE *f);

int opt_i;
int opt_v;

int
main(int argc, char *argv[])
{
    regex_t pat;
    int err, i;
    int opt;
    
    while ( (opt = getopt(argc,argv,"iv")) != -1)
    {
        switch(opt)
        {
            case 'i':                
                opt_i = 1;
                break;
            case 'v':                
                opt_v = 1;
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-iv] [<file>...]\n", argv[0]);
                exit(1);
        }
    }
    
    argc -= optind;
    argv += optind;    
    // at this time
    // argv[0] : patern
    // argv[1] : filename
    
    for( i = 0 ; i < argc ; i++)
    {
        fprintf(stdout,"argv[%d] : %s\n",i,argv[i]);
    }
    
    if( argc < 2)
    {
        fputs("no pattern OR no file\n",stderr);
        exit(1);
    }
    int reg_mode = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
    if( opt_i)
        reg_mode |= REG_ICASE;
    err = regcomp(&pat, argv[0], reg_mode);
    if( err != 0)
    {
        char buf[1024];

        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }

    if( argc == 1)
    {
        do_grep(&pat, stdin);
    }
    else
    {
        for( i =1 ; i<argc ; i++)
        {
            FILE *f;
            f = fopen(argv[i],"r");
            if( !f )
            {
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f);
            fclose(f);
        }
    }
    regfree(&pat);
    exit(0);
}
static void
do_grep(regex_t *pat, FILE *src)
{
    char buf[4096];
    int matched;
    while( fgets(buf, sizeof buf, src))
    {
        matched = ( regexec(pat, buf, 0, NULL, 0) == 0);
        if( opt_v)    
            matched = !matched;
        if(matched)
            fputs(buf, stdout);
    }
}