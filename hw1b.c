#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

#define INTERVAL_SECS 		1
#define INTERVAL_MICROSECS 	0
#define VALUE_SECS 		1
#define VALUE_MICROSECS 	0

/*
    if (argc > 1) {
        printf("Usage: %s takes no arguments, ctrl-c to quit\n", argv[0]);
        exit(0);
    }
     */


//function definitions
void alarmHandler(int value);
void setrtimer(struct itimerval *);
void readProcFile();
void errorMessage();

//global variables for tracking seconds
float processesLastSecond = -1;
float processesSinceBoot = -1;

int main(int argc, char *argv[]) {
    struct itimerval realt;

    signal(SIGALRM, alarmHandler); //add error for signal alarm thing

    //register signal and set alarm
    setrtimer(&realt);

    if (setitimer(ITIMER_REAL, &realt, NULL) == -1) {
        errorMessage();
    }

    printf("stop 3\n");

    while (1) {
        printf("stop 4\n");
        int pauseValue = pause(); //TODO: figure out why this line is a huge problem...???

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
    signal(SIGALRM, alarmHandler);
}

/*
  Initialize the ITIMER_REAL interval timer.
  Its interval is one second.  Its initial value is one second.
*/

//void setrtimer(struct itimerval *ivPtr) {
//    ivPtr->it_interval.tv_sec = INTERVAL_SECS;
//    ivPtr->it_interval.tv_usec = INTERVAL_MICROSECS;
//    ivPtr->it_value.tv_sec = VALUE_SECS;
//    ivPtr->it_interval.tv_usec = VALUE_MICROSECS; //TODO: understand why this doesn't work!??!!
//}

void setrtimer(struct itimerval *ivPtr) {
    ivPtr->it_value.tv_sec = INTERVAL_SECS;
    ivPtr->it_value.tv_usec = INTERVAL_MICROSECS;
    ivPtr->it_interval = ivPtr->it_value;
}

void readProcFile() {
    int currentChar = 0;
    FILE *file;

    file = fopen("/proc/stat", "r");

    if (file != NULL) {
        //have to get to the right place in the stat file, so that next pointer is to a numerical value
        while (currentChar != 'r') {
            currentChar = fgetc(file);
        }

        //read int in to variable value and check for errors
        if (fscanf(file, "%f", &processesSinceBoot) <= EOF) {
            errorMessage();
        } else {
            printf("Processes since boot time: %f\n", processesSinceBoot);

            //have to have logged value for processes since boot to do the computation
            if (processesLastSecond != -1) {
                printf("Processes in last second: %f\n",
                       processesSinceBoot - processesLastSecond); //TODO: check computation correct with TA's
            }

            //keep the boot value for next computation
            processesLastSecond = processesSinceBoot;
        }

        //close the file after finished reading the information
        fclose(file);
    } else {
        errorMessage();
    }

    printf("test loop");
}

void errorMessage() {
    printf("unforeseen error occurred\n");
    exit(1);
}

/*
 * setitimer.c - simple use of the interval timer
 */

#include <sys/time.h>		/* for setitimer */
#include <unistd.h>		/* for pause/* function prototype */
void DoStuff(void);
void setitimerVal(struct itimerval *ivPtr);

//int main(int argc, char *argv[]) {
//
//    struct itimerval it_val;	/* for setting itimer */
//
//    /* Upon SIGALRM, call DoStuff().
//     * Set interval timer.  We want frequency in ms,
//     * but the setitimer call needs seconds and useconds. */
//    if (signal(SIGALRM, (void (*)(int)) DoStuff) == SIG_ERR) {
//        perror("Unable to catch SIGALRM");
//        exit(1);
//    }
//
//    setitimerVal(&it_val);
//
//    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
//        perror("error calling setitimer()");
//        exit(1);
//    }
//
//    while (1)
//        pause();
//
//}
//
//void setitimerVal(struct itimerval *ivPtr){
//    ivPtr->it_value.tv_sec =     INTERVAL_SECS;
//    ivPtr->it_value.tv_usec =    INTERVAL_MICROSECS;
//    ivPtr->it_interval = ivPtr->it_value;
////    ivPtr->it_interval.tv_sec = 1;
////    ivPtr->it_interval.tv_usec = 0;
////    ivPtr->it_interval = ivPtr->it_value;
//}
//
///*
// * DoStuff
// */
//void DoStuff(void) {
//
//    printf("Timer went off.\n");
//
//} */
#include <signal.h>		/* for signal */

#define INTERVAL 500		/* number of milliseconds to go off */

//