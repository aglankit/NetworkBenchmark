#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int sockfd = 0;
    int n, index = 0;
    struct sockaddr_in serv_addr, remaddr;
    socklen_t addrlen = sizeof(remaddr);
    char* read_buf;
    char* send_buf;
    char* temp_buf;

    struct timespec t1, t2;
    double elaspedtime;
    int buffer_size = atoi(argv[1]);
    read_buf = (char *)calloc(buffer_size, sizeof(char));    
    send_buf = (char *)calloc(buffer_size, sizeof(char));    

    srand(time(NULL));

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&serv_addr, '\0', sizeof(serv_addr));
    temp_buf = send_buf;
    for (n = 0; n < buffer_size - 1; n++)
    {
        *temp_buf = 'A' + (random() % 26);
        temp_buf++;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(55555);

    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while(1)
    {
        int count = 0;
        int n = 0;
        while ((n = recvfrom(sockfd, read_buf, buffer_size, 0, (struct sockaddr *)&remaddr, &addrlen)) >= 0) {
            if (n == (buffer_size))
            {
                sendto(sockfd, send_buf, buffer_size, 0, (struct sockaddr *)&remaddr, sizeof(remaddr));
            }
        }
    }
    close(sockfd);

    return 0;
}
