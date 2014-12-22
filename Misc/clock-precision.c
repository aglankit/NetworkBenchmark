#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    struct timespec t1, t2;
    double elaspedtime;
    int i;

    clock_gettime(CLOCK_MONOTONIC, &t1);

    clock_gettime(CLOCK_MONOTONIC, &t2);
    elaspedtime = difftime(t1, t2);
    elaspedtime = (t2.tv_sec - t1.tv_sec)*1000000;
    elaspedtime += (t2.tv_nsec - t1.tv_nsec);
    printf("Time Elasped %lf\n", elaspedtime);
}
