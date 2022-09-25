# **VASH** - Vicious A3gis Shell
aka Very Amazing Shell :)

## What it is
VASH is a shell coded in C that emulates the behaviour of BASH (hence the similar name)

## How to use
Download the files and put them in a directory. Use the <code>cd</code> command to navigate to the directory, and then run <code> make </code>.
This will create an executable <code>vash</code>, which you can run using the command <code>./vash</code>.
Please run <code>chmod +x vash</code> to get the required execture permissions. VASH would start in the same terminal.

## Features
- Implementation of semicolon-seperated commands

- Supports pipe commands

- Supports I/O redirection

- Implementations of cd, echo, pwd etc.

- Foreground and background processes

- Autocomplete

- Jobs command showing background processes

- History command

- Pinfo command which shows information about a process

- Discover command which finds instances of the search term

## Files

### **main.c**
clear display, show initial prompt and take input. Also handles the autocomplete functionality.

### **inputHandler.c**
parse the input to seperate commands and arguments, handle i/o redirection.

### **trivial_functions.c**
contains functions being used across the program

### **variables.c**
setting global variables

### **cd.c**
implementation of cd, supports all flags.

### **echo.c**
implementation of echo, taking care of spaces and tabs

### **prompt.c**
Contains the initial prompt and prompt to be shown every time

### **discover.c**
All functions related to discover command. Does not search for or display hidden files.

### **syscmd.c**
Contains code to run foreground and background processes using execvp.
Time taken shown in next promt is time taken by last foreground process to finish.
Also containts code to show jobs, and perform the fg and bg commands.

### **pinfo.c**
Contains code to display information regarding required process.

### **pipe.c**
Contains code to handle multiple pipes in input

### **history.c**
Contains code that handles history of commands entered. These commands are stored in `cmd_history.txt` to make history work between multiple sessions.

### **signalHandler.c** 
Contains code to handle ctrl-c and ctrl-z signals and communicate with child processes. Also implements the sig command.

### **headers.h**
Contains all the required headers for all files
