#include "headers.h"

void prompt() {
    printf("vash ");
    char name[100];

    struct passwd *passwd = getpwuid(getuid());
    
    gethostname(name, 100);
    printf("<%s@%s:%s>$ ", passwd->pw_name , name);
}
