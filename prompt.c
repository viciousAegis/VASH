#include "headers.h"

void prompt(path directory) {
    //name of shell
    printf("%s ", vash);

    struct passwd *passwd = getpwuid(getuid());

    input hostnameBuffer = (input) calloc(1024, sizeof(char));
    gethostname(hostnameBuffer, 100);
    printf("<%s@%s:%s>$ ", passwd->pw_name , hostnameBuffer, directory);
}
