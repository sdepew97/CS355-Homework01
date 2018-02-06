#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <memory.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define INTERVAL_SECS 		1
#define INTERVAL_MICROSECS 	0

char *correctInput = "-s\0";

//function definitions
void alarmHandler(int value);
void setrtimer(struct itimerval *ivPtr, int seconds, int microsecs);
void parseCmd(int argc, char *argv[], int *sec);
void readProcFile(int sec);
void errorMessage();

//global variables for tracking seconds
long int processesLastSecond = -1;
long int processesSinceBoot = -1;

int main(int argc, char *argv[]) {


    struct itimerval realt;

    //TODO: print instructions on use
    /*
    if (argc > 1) {
        printf("Usage: %s takes no arguments, ctrl-c to quit\n", argv[0]);
        exit(0);
    }
     */

    signal(SIGALRM, alarmHandler); //add error for signal alarm thing

    //register signal and set alarm
    int sec = INTERVAL_SECS;
    int microsec = INTERVAL_MICROSECS;
    if(argc == 1) {
        //call normally
        setrtimer(&realt, sec, microsec);
    }
    else if(argc == 3) { //number of arguments should either be one or three, but nothing else
        //set sec and microsec differently based on commandline input
        parseCmd(argc, argv, &sec);
        setrtimer(&realt, sec, microsec);
    }
    else {
        errorMessage();
    }

    if (setitimer(ITIMER_REAL, &realt, NULL) == -1) {
        errorMessage();
    }

    while (1) {
        int pauseValue = pause();

        if (pauseValue == -1) {
            readProcFile(sec);
        } else {
            errorMessage();
        }
    }

    return 0;
}

//signal handler
void alarmHandler(int value) {
    //reset signal handler //TODO: Check what should be done here... //done
    signal(SIGALRM, alarmHandler);
}

/*
  Initialize the ITIMER_REAL interval timer.
  Its interval is one second.  Its initial value is one second.
*/
void setrtimer(struct itimerval *ivPtr, int seconds, int microsecs) {
    ivPtr->it_value.tv_sec = seconds;
    ivPtr->it_value.tv_usec = microsecs;
    ivPtr->it_interval = ivPtr->it_value;
//    ivPtr->it_interval.tv_sec = seconds;
//    ivPtr->it_interval.tv_usec = ;
}

void parseCmd(int argc, char *argv[], int *sec){
    if(strcmp(argv[1], correctInput) == 0){
        //check that input is an integer
        char *ptr;
        long ret;

        ret = strtol(argv[2], &ptr, 10); //TODO: ask dianna about using this function

        if(ret != EINVAL && ret != ERANGE) { //TODO: error check
            *sec = ret;
        }
    }
    else {
        errorMessage();
    }
}

void readProcFile(int sec) {
    int currentChar = 0;
    FILE *file;

    file = fopen("/proc/stat", "r");

    if (file != NULL) {
        //have to get to the right place in the stat file, so that next pointer is to a numerical value
        while (currentChar != 'r') {
            currentChar = fgetc(file);
        }

        //read int in to variable value and check for errors
        if (fscanf(file, "%ld", &processesSinceBoot) <= EOF) { //strtok() and atoi()
            errorMessage();
        } else {
            printf("Processes since boot time: %ld\n", processesSinceBoot);

            //have to have logged value for processes since boot to do the computation
            if (processesLastSecond != -1) {
                printf("Processes in last %d seconds: %ld\n", sec,
                       processesSinceBoot - processesLastSecond); //TODO: check computation correct with TA's //done
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

void errorMessage() {
    printf("unforeseen error occurred\n");
    exit(1);
}
