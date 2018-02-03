//
// Created by Sarah Depew on 2/2/18.
//

/*
• Using only system calls (that is, functions documented in section 2 of the Unix/Linux
  manual pages such as fork(), execl(), execve(), open(), read(), write(), wait(), etc.,
  write a C program, microcat, that concatenates and prints files (taken as arguments)
  to standard output. If no arguments are given, it takes input from stdin. This is
  basically the Unix cat with no flags. Remember that stdin is file descriptor 0, and
  stdout is file descriptor 1.
• Modify microcat so that if a signal arrives it prints the message “Help! I think I’ve
  been shot!!!” before terminating.
• Important: You may use only UNIX system calls (those documented in section 2 of
  the UNIX man pages, plus any of waitpid(), sleep(), signal(), kill(), exit() and select()
  (which are not strictly system calls on some systems)) to implement this program. You may
  NOT use the standard library functions (higher-level functions documented in section 3
  of the manual).
*/

#include <stdio.h>

//function defintions
void errorMessage();

//global variables

int main(int argc, char *argv[]){

    //one input value, then take from stdin
    if(argc == 1){

    }

    //cat the files that are input!
    else if(argc > 1) {

    }

    //an error occured, so print a message to this effect
    else {
        errorMessage();
    }

    return 0;
}

void errorMessage()
{
    printf("unforeseen error occurred");
}
