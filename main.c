#include "headers.h"

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

void autocomplete()
{
    char* input = (char*) calloc(1024, sizeof(char));
    strcpy(input, bufferInput);


    char* token = strtok(input, " ");

    token = strtok(NULL, " ");

    initFileNames();
    initMyDir(".");

    fileName* matchedFiles = (fileName*) calloc(1024, sizeof(fileName));
    int matchCount = 0;

    if(token == NULL)
    {
        printf("\n");
        for(int i = 0; i < lsFileCount; i++)
        {
            if(areDirs[i])
            {
                printf("%s/\n", fileNames[i]);
                continue;
            }
            printf("%s\n", fileNames[i]);
        }
        prompt(currDirectory);
        printf("%s", bufferInput);
        return;
    }

    for(int i = 0; i < lsFileCount; i++)
    {
        if(!strncmp(token, fileNames[i], strlen(token)))
        {
            matchedFiles[matchCount] = (fileName) calloc(1024, sizeof(char));
            if(areDirs[i])
            {
                sprintf(matchedFiles[matchCount++], "%s/", fileNames[i]);
                continue;
            }
            strcpy(matchedFiles[matchCount++], fileNames[i]);
        }
    }

    // for one file
    if(matchCount == 1)
    {
        char* toComplete = (char*) calloc(1024, sizeof(char));
        strcpy(toComplete, matchedFiles[0] + strlen(token));
        if(strchr(toComplete, '/'))
        {
            printf("%s", toComplete);
        }
        else
        {
            printf("%s ", toComplete);
        }
        sprintf(bufferInput, "%s%s", bufferInput, toComplete);

        return;
    }

    // multipleFiles
    printf("\n");
    for(int i = 0; i < matchCount; i++)
    {
        printf("%s\n", matchedFiles[i]);
    }
    prompt(currDirectory);
    printf("%s", bufferInput);
}

int main()
{
    resetRedirection();

    clearDisplay();

    initialPrompt();

    // load the history file
    loadHistory();

    //background PIDs List
    backgroundPIDs = LL_init();

    rootPath = (path) calloc(1024, sizeof(char));
    getcwd(rootPath, 1024);

    // initialise the current directory as the root directory
    currDirectory = rootPath;

    //initialse previous directory as the root directory
    prevDirectory = rootPath;

    childCaught = 0;

    // initialise number of background processes
    processCount = 0;

    char c;

    while (1)
    {
        //check for signals
        catchSignals();

        setbuf(stdout, NULL);
        enableRawMode();
        
        //check which directory user is in and prompt user
        currDirectory = checkDirectory();
        prompt(currDirectory);

        bufferInput = (input) calloc(1024, sizeof(char));

        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (iscntrl(c)) {
                if (c == 10)
                {
                    printf("\n");
                    break;
                }
                else if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (bufferInput[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        bufferInput[--pt] = '\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                    autocomplete();
                } else if (c == 4) {
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                bufferInput[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

        handleInput();
    }
}