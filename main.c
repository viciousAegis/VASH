#include "headers.h"

int main()
{
    clearDisplay();

    // load the history file
    
    initialPrompt();

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

    while (1)
    {
        //check for signals
        catchSignals();
        
        //reset the redirections
        resetRedirection();

        //check which directory user is in and prompt user
        currDirectory = checkDirectory();
        prompt(currDirectory);


        bufferInput = (input) calloc(1024, sizeof(char));
        if (fgets(bufferInput, 1024, stdin) != NULL){
            handleInput();
        } else {
            exit(0);
        }
    }
}
