#include <sys/socket.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
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
    double elaspedtime, overall_elasped_time;
    struct timespec t1, t2;
    struct sockaddr_in serv_addr;
    int buffer_size = atoi(argv[1]);
    int flag = 1;
    int result;
    int bytes_read = 0;
    int loops;

    recvbuf = (char *)calloc(buffer_size, sizeof(char));
    sendbuf = (char *)calloc(buffer_size, sizeof(char));
    srand(time(NULL));

    memset(&serv_addr, '\0', sizeof(serv_addr));

    tempbuf = sendbuf;
    for (n = 0; n < buffer_size - 1; n++)
    {
        *tempbuf = 'A' + (random() % 26);
        tempbuf++;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
    }

    result = setsockopt(sockfd,            /* socket affected */
                        IPPROTO_TCP,     /* set option at TCP level */
                        TCP_NODELAY,     /* name of option */
                        (char *) &flag,  /* the cast is historical
                                                         cruft */
                        sizeof(int));    /* length of option value */

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(55555);
    

    if(inet_pton(AF_INET, "128.105.112.137", &serv_addr.sin_addr) <= 0)
    {
        printf("Inet_Pton error occurred\n");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    overall_elasped_time = 1000000000;

    for (loops = 0; loops <= 1000; loops++) {    
        bytes_read = 0; 
        clock_gettime(CLOCK_MONOTONIC, &t1);

        while ((n = recv(sockfd, recvbuf, buffer_size, 0)) > 0)
        {
            bytes_read += n;
            if (bytes_read == buffer_size)
                break;
        }

        clock_gettime(CLOCK_MONOTONIC, &t2);
        elaspedtime = (t2.tv_sec - t1.tv_sec)*1000000000;
        elaspedtime += (t2.tv_nsec - t1.tv_nsec);

        if (elaspedtime < overall_elasped_time)
            overall_elasped_time = elaspedtime;
    }

    printf("Minimum elasped time = %lf\n", overall_elasped_time);
    close(sockfd);
}
