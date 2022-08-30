#include "headers.h"

void clearDisplay()
{
	write(1, "\e[1;1H\e[2J", 10);
}

path checkDirectory()
{
    path relativePath;

    path newDirectory = (path) calloc(1024, sizeof(char));
    path directory = (path) calloc(1024, sizeof(char));

    getcwd(directory, 1024);
    
    int flag = strcmp(rootPath, directory);

    if(flag > 0)
    {
        //handle the case of parent directory
    }
    else if(flag < 0)
    {
        relativePath = (path) calloc((strlen(directory) - strlen(rootPath)), sizeof(char));
        strncpy(relativePath, directory + strlen(rootPath), strlen(directory) - strlen(rootPath));
        sprintf(newDirectory, "~%s", relativePath);
    }
    else
    {
        sprintf(newDirectory, "~");
    }

    return newDirectory;
}

void performExit()
{

        printf("Thank you for using VASH :)... Now exiting\n.\n.\n.\n");
        printf("Vicious A3gis Shell © Akshit Sinha, 2022 \n");
        exit(0);
}