#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
//only for error message for error handling
#include <stdlib.h>

//function definitions
void signalHandler(int value);
void writeFileToStdout(char *fileName);
void errorMessage();

int main(int argc, char *argv[]){

    //register the signal handlers using a loop
    for(int i=SIGHUP; i<=SIGSYS; i++) {
        signal(i, signalHandler);
    }

   //alarm(2); //TODO: check on error

    //one input value, then take from stdin
    if(argc == 1) {
        char c;

        //TODO: check with dianna about buffer...
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

/*
 * Signal handler that exits whenever a signal is caught
 */
void signalHandler(int value) {
    write(STDOUT_FILENO, "\nHelp! I think I've been shot!!!\n\0", 33);
    exit(EXIT_SUCCESS);
}

/*
 * Read file contents and print out to stdout in a concatenated manner
 */
void writeFileToStdout(char *fileName) {
    //open the file to read
    int fileDescriptor = open(fileName, O_RDWR);

    //check that the file open worked correctly
    if (fileDescriptor != -1) {
        //output contents to the terminal/stdout one byte at a time
        char c;
        while (read(fileDescriptor, &c, 1) > 0) { //check that read was successful
            if (write(1, &c, 1) < 0) { //check that error did not occur with the write
                errorMessage();
            }
        }

        //close the file
        close(fileDescriptor);

    } else {
        errorMessage();
    }
}

/*
 * Error method to handle any error in program
 */
void errorMessage()
{
    write(1, "\nUnforeseen error occurred.\n\0", 28);
    exit(EXIT_FAILURE);
}
