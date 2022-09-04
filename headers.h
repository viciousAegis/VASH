#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <time.h>   
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/times.h>

#include "prompt.h"
#include "echo.h"
#include "variables.h"
#include "trivial_functions.h"
#include "inputHandler.h"
#include "pwd.h"
#include "cd.h"
#include "ls.h"
#include "syscmd.h"
#include "history.h"

#endif