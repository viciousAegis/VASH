#include "headers.h"

int main()
{
    clearDisplay();

    initialPrompt();

    rootPath = (path) calloc(1024, sizeof(char));
    getcwd(rootPath, 1024);

    // initialise the current directory as the root directory
    currDirectory = rootPath;

    //initialse previous directory as the root directory
    prevDirectory = rootPath;

    while (1)
    {
        //check which directory user is in and prompt user
        currDirectory = checkDirectory();
        prompt(currDirectory);

        bufferInput = (input) calloc(1024, sizeof(char));
        fgets(bufferInput, 1024, stdin);

        handleInput();
    }
}
