# **VASH** - Vicious A3gis Shell
aka Very Amazing Shell :)

## What it is
<hr>
VASH is a shell coded in C that emulates the behaviour of BASH (hence the similar name)

## How to use
<hr>
Download the files and put them in a directory. Use the <code>cd</code> command to navigate to the directory, and then run <code> make </code>.
This will create an executable <code>vash</code>, which you can run using the command <code>./vash</code>.
Please run <code>chmod +x vash</code> to get the required execture permissions. VASH would start in the same terminal.

## Features
<hr>

- Implementation of semicolon-seperated commands

- Implementations of cd, echo, pwd etc.

## Files
<hr>

### **main.c**
clear display, show initial prompt and take input.

### **inputHandler.c**
parse the input to seperate commands and arguments

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

### **headers.h**
Contains all the required headers for all files
