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
  been shot!!!” before terminating. //TODO: understand how to catch any signal... //TODO register a handler for all signals
• Important: You may use only UNIX system calls (those documented in section 2 of
  the UNIX man pages, plus any of waitpid(), sleep(), signal(), kill(), exit() and select()
  (which are not strictly system calls on some systems)) to implement this program. You may
  NOT use the standard library functions (higher-level functions documented in section 3
  of the manual).
*/

//TODO: bug for control d termination,

#include <unistd.h>
#include <fcntl.h>

//only for error message for error handling //TODO: check if we can use methods not system calls for error handling
#include <stdlib.h>

//function definitions
void signalHandler(int value);
void writeFileToStdout(char *fileName);
void errorMessage();

//global variables

int main(int argc, char *argv[]){

    //register the signal handlers //TODO go through loop for all signals
    for(int i=1; i<32; i++) { //register all signal handlers using a loop
        signal(i, signalHandler);
    }

   //alarm(2);

    //one input value, then take from stdin
    if(argc == 1) {
        char c;

        //TODO fix weird printing bug!! it prints it out even after termination //flush then terminate
        while (read(0, &c, 1) > 0) {
            if (c != '\n') {
                if (write(1, &c, 1) < 0) {
                    errorMessage();
                }
            } else {
                if (write(1, "\n", 1) < 0) {
                    errorMessage();
                }
            }
        }
    }

    //cat the files that are input!
    else if(argc > 1) {
        //print out files that are input after reading them and outputting their contents to the terminal!
        for(int i=1; i<argc; i++) {
            writeFileToStdout(argv[i]); //write the files one at a time to stdout
        }
    }

    //an error occurred, so print a message to this effect
    else {
        errorMessage();
    }

    return 0;
}

//signal handler
void signalHandler(int value) {
    write(STDOUT_FILENO, "\nHelp! I think I've been shot!!!\n\0", 33);
    exit(0);
}

//read file contents and print out to stdout
void writeFileToStdout(char *fileName) {
    //open the file to read
    int fileDescriptor = open(fileName, O_RDWR); //TODO: check flag and functionality

    if (fileDescriptor!=-1) {
        //output contents to the terminal/stdout
        char c;
        while (read(fileDescriptor, &c, 1) > 0) {
            if(write(1, &c, 1)<0) {
                errorMessage();
            }
        }

        //close the file
        close(fileDescriptor);

    } else {
        errorMessage();
    }
}

void errorMessage()
{
    write(1, "\nUnforeseen error occurred.\n\0", 30);
    exit(1);
}
