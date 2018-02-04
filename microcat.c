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
  been shot!!!” before terminating. //TODO: understand how to catch any signal...
• Important: You may use only UNIX system calls (those documented in section 2 of
  the UNIX man pages, plus any of waitpid(), sleep(), signal(), kill(), exit() and select()
  (which are not strictly system calls on some systems)) to implement this program. You may
  NOT use the standard library functions (higher-level functions documented in section 3
  of the manual).
*/

#include <unistd.h>
#include <fcntl.h>

//only for error message for error handling
#include <printf.h>
#include <stdlib.h>

//function definitions
void writeFileToStdout(char *fileName);
void errorMessage();

//global variables

int main(int argc, char *argv[]){

    //one input value, then take from stdin
    if(argc == 1){
        int c;

        while(1) {
            read(0, &c, 1);
            if(c != '\n') {
                write(1, &c, 1);
            }
            else {
                write(1, "\n", 1);
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
    printf("unforeseen error occurred");
    exit(1);
}
