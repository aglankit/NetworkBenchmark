#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0, i = 0;
    char* sendbuf;
    char* recvbuf;
    char* tempbuf;
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof(serv_addr);
    double elaspedtime, minimum_elasped_time;
    struct timespec t1, t2;
    int count = 0;
    int buffer_size = atoi(argv[1]);
    srand(time(NULL));
    int num_packets = 0;

    recvbuf = (char *)calloc(buffer_size, sizeof(char));
    sendbuf = (char *)calloc(buffer_size, sizeof(char));

    memset(&serv_addr, '\0', sizeof(serv_addr));

    tempbuf = sendbuf;
    for (n = 0; n < buffer_size - 1; n++)
    {
        *tempbuf = 'A' + (random() % 26);
        tempbuf++;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(55555);
    serv_addr.sin_addr.s_addr = inet_addr("128.105.112.137");

    minimum_elasped_time = 10000000;
 
    for(i =0; i<1000; i++) {
        clock_gettime(CLOCK_MONOTONIC, &t1);
        elaspedtime = 0;
        sendto(sockfd, sendbuf, buffer_size, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        if ((n = recvfrom(sockfd, recvbuf, buffer_size, 0, (struct sockaddr *)&serv_addr, &addrlen)) > 0) {
            if (n == buffer_size) {
                clock_gettime(CLOCK_MONOTONIC, &t2);
                elaspedtime = (t2.tv_sec - t1.tv_sec)*1000000000;
                elaspedtime += (t2.tv_nsec - t1.tv_nsec);
                if (elaspedtime < minimum_elasped_time)
                    minimum_elasped_time = elaspedtime;
            }
        }
    }

    printf("Latency %lf for packet size %d\n", minimum_elasped_time, n);

    close(sockfd);
}
