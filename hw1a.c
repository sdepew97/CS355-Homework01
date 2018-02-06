#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

//function definitions
void alarmHandler(int value);
void readProcFile();
void errorMessage();

//global variables for tracking seconds
long int processesLastSecond = -1;
long int processesSinceBoot = -1;

int main(int argc, char *argv[])
{
    //register signal and set alarm
    signal(SIGALRM, alarmHandler);
    alarm(1); //register first alarm to start us off...

    //stay in a loop to catch the alarm values and pauses 
    while(1) {
        int pauseValue = pause();

	//want to have the file read when pause "fails" and returns the error value of -1
        if (pauseValue == -1) {
            readProcFile();
        }
	//otherwise the signal was not caught and so an error should be thrown
	else {
            errorMessage();
        }
    }

    return 0;
}

//signal handler
void alarmHandler(int value) {
    //set up next alarm call
    int valueRemaining = alarm(1);

    if(valueRemaining!=0)
        errorMessage();
}

/*
 * Method to read the computer's proc file and parse the number of interrupts 
 */
void readProcFile() {
    int currentChar = 0;
    FILE *file;

    //Open the file
    file = fopen("/proc/stat", "r");

    if (file != NULL) {
        //have to get to the right place in the stat file, so that next pointer is to a numerical value
        while (currentChar != 'r') {
            currentChar = fgetc(file);
        }

        //read int in to variable value and check for errors
        if (fscanf(file, "%ld", &processesSinceBoot) <= EOF) {
            errorMessage();
        } else {
            printf("Processes since boot time: %ld\n", processesSinceBoot);

            //have to have logged value for processes since boot to do the computation
            if (processesLastSecond != -1) {
                printf("Processes in last second: %ld\n",
                       processesSinceBoot - processesLastSecond);
            }

            //keep the boot value for next computation
            processesLastSecond = processesSinceBoot;
        }

        //close the file after finished reading the information
        fclose(file);
    } else {
        errorMessage();
    }
}

/*
 * Error method to handle any error in program
 */
void errorMessage()
{
    printf("Unforeseen error occurred.\n");
    exit(EXIT_FAILURE);
}
