#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int globalInteruptsSinceLastSecond;
int globalTotalInterupts;

/*
 * Write a program that will show the rate of interrupts.
 * Each second it should print the number of interrupts (1)
 * in the previous second, and (2) the total number of
 * interrupts since boot time.
 */

//signal handler
void alarmHandler(int value);
void readProcFile();
void errorMessage();

float processesLastSecond = -1;
float processesSinceBoot = -1;

int main(void)
{
    //register signal and set alarm
    signal(SIGALRM, alarmHandler);
    alarm(1); //alarm to start us off...

    while(1) {
        int pauseValue = pause();

        if (pauseValue == -1) {
            readProcFile();
        } else {
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

void readProcFile()
{
    int currentChar = 0;
    FILE *file;

    file = fopen("/proc/stat", "r");

    if(file!=NULL)
    {
        //have to get to the right place in the stat file, so that next pointer is to a numerical value
        while(currentChar != 'r') {
            currentChar = fgetc(file);
        }

        //read int in to variable value and check for errors
        if(fscanf(file, "%f", &processesSinceBoot) <= EOF) {
            errorMessage();
        }
        else {
            printf("Processes since boot time: %f\n", processesSinceBoot);

            //have to have logged value for processes since boot to do the computation
            if(processesLastSecond != -1) {
                printf("Processes in last second: %f\n", processesSinceBoot - processesLastSecond); //TODO: check computation correct with TA's
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

void errorMessage()
{
    printf("error occurred in program");
}