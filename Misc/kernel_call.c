#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    struct timespec t1, t2, t3;
    double elaspedtime;
    int i;

    struct timeval ttemp;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    getpid();
    // stop timer
    clock_gettime(CLOCK_MONOTONIC, &t2);
    elaspedtime = (t2.tv_sec - t1.tv_sec)*1000000;
    elaspedtime += (t2.tv_nsec - t1.tv_nsec);
    printf("Time Elasped %lf\n", elaspedtime);
}
