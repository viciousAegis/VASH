#include "headers.h"

void initialPrompt()
{
    int fd = open("./ascii.txt", O_RDONLY);
    char buf[1024];
    int buflen;
    while((buflen = read(fd, buf, 1024)) > 0)
    {
        write(1, buf, buflen);
    }
    close(fd);

    timeElapsedSinceChildStart = 0;
}

void prompt(path directory) {

    // never write prompt to file
    if(isOutputRedirected || isOutputAppended)
    {
        close(outputFile);
        open(1 , O_WRONLY);
    }

    printf("\033[0;35m");
    printf("%s ", vash);

    struct passwd *passwd = getpwuid(getuid());

    input hostnameBuffer = (input) calloc(1024, sizeof(char));
    gethostname(hostnameBuffer, 100);

    printf("\033[1;32m");
    printf("<%s@%s:", passwd->pw_name, hostnameBuffer);

    printf("\033[0;36m");
    printf("%s", directory);

    if(timeElapsedSinceChildStart > 0)
    {
        printf("\033[1;33m");
        printf("took %ds", timeElapsedSinceChildStart);
    }

    printf("\033[1;32m");
    printf(">$ ");
    printf("\033[0m");

    timeElapsedSinceChildStart = 0;
}
