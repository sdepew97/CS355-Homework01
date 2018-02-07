#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <memory.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INTERVAL_SECS 		1
#define INTERVAL_MICROSECS 	0

//global variables
char *correctInput = "-s\0";

//global variables for tracking seconds
long int processesLastSecond = -1;
long int processesSinceBoot = -1;

//function definitions
void alarmHandler(int value);
void setrtimer(struct itimerval *ivPtr, int seconds, int microsecs);
void parseCmd(int argc, char *argv[], long long int *sec);
void readProcFile(int sec);
void errorMessage();

int main(int argc, char *argv[]) {
    //local variables for main
    long long int sec = INTERVAL_SECS;
    long long int microsec = INTERVAL_MICROSECS;
    struct itimerval realt;

    //register signal and check successful
    if(signal(SIGALRM, alarmHandler) == SIG_ERR) {
        errorMessage();
    }

    //number of arguments should either be one or three, but nothing else
    if (argc == 1) { //one input
        //call normally
        setrtimer(&realt, sec, microsec);
    } else if (argc == 3) { //call with terminal flag and input
        //set sec and microsec differently based on commandline input
        parseCmd(argc, argv, &sec);
        setrtimer(&realt, sec, microsec);
    } else {
        errorMessage();
    }

    //check that the setitimer was set properly and if not raise an error message
    if (setitimer(ITIMER_REAL, &realt, NULL) == -1) {
        errorMessage();
    }

    //wait for signals in the while loop
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

/*
 * Signal Handler that resets the signal() function
 */
void alarmHandler(int value) {
    //reset signal handler
    signal(SIGALRM, alarmHandler);
}

/*
  Initialize the ITIMER_REAL interval timer.
  Its interval is one second. Its initial value is one second.
*/
void setrtimer(struct itimerval *ivPtr, int seconds, int microsecs) {
    ivPtr->it_value.tv_sec = seconds;
    ivPtr->it_value.tv_usec = microsecs;
    ivPtr->it_interval.tv_sec = seconds;
    ivPtr->it_interval.tv_usec = microsecs;
}

/*
 * Parse the command line input and ensure that the user has put in the correct information
 */
void parseCmd(int argc, char *argv[], long long int *sec){
    //check that -s flag was input correctly, otherwise error
    if(strcmp(argv[1], correctInput) == 0) {
        //check that input is correctly an integer
        //(error checking gotten from stack overflow)
        const char *nptr = argv[2];                     /* string to read               */
        char *endptr = NULL;                            /* pointer to additional chars  */
        int base = 10;                                  /* numeric base (default 10)    */
        long long int number = 0;                       /* variable holding return      */

        /* reset errno to 0 before call */
        errno = 0;

        /* call to strtol assigning return to number */
        number = strtoll(nptr, &endptr, base);

        /* test return to number and errno values */
        if (nptr == endptr) {
            printf(" number : %lld  invalid  (no digits found, 0 returned)\n", number);
            errorMessage();
        } else if (errno == ERANGE && number == LONG_MIN) {
            printf(" number : %lld  invalid  (underflow occurred)\n", number);
            errorMessage();
        } else if (errno == ERANGE && number == LONG_MAX) {
            printf(" number : %lld  invalid  (overflow occurred)\n", number);
            errorMessage();
        } else if (errno == EINVAL) { /* not in all c99 implementations - gcc OK */
            printf(" number : %lld  invalid  (base contains unsupported value)\n", number);
            errorMessage();
        }
        else if (errno != 0 && number == 0) {
            printf(" number : %lld  invalid  (unspecified error occurred)\n", number);
            errorMessage();
        }
        else if (errno == 0 && nptr && *endptr != 0) {
            printf(" number : %lld    invalid  (since additional characters remain)\n", number);
            errorMessage();
        }

        //passed all the if statements and so can assigned the seconds value as the one input
        *sec = number;
    }
    else {
        errorMessage();
    }
}

/*
* Method to read the computer's proc file and parse the number of interrupts
*/
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
        if (fscanf(file, "%ld", &processesSinceBoot) <= EOF) {
            errorMessage();
        } else {
            printf("Processes since boot time: %ld\n", processesSinceBoot);

            //have to have logged value for processes since boot to do the computation
            if (processesLastSecond != -1) {
                printf("Processes in last %d seconds: %ld\n", sec,
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
