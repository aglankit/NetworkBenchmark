#include <sys/socket.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd =0;
    int n, index = 0;
    struct sockaddr_in serv_addr;
    char* read_buf;
    char* send_buf;
    char* temp_buf;
    int flag = 1;
    int result;
    int buffer_size = atoi(argv[1]);
    int bytes_read = 0;
    int loops = 0;
    read_buf = (char *)calloc(buffer_size, sizeof(char));    
    send_buf = (char *)calloc(buffer_size, sizeof(char));    

    srand(time(NULL));

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
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

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);

    while ((connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) < 0);

    result = setsockopt(connfd,            /* socket affected */
                            IPPROTO_TCP,     /* set option at TCP level */
                            TCP_NODELAY,     /* name of option */
                            (char *) &flag,  /* the cast is historical
                                                         cruft */
                            sizeof(int));    /* length of option value */

    while(1) {
    bytes_read = 0; 
    while ((n = recv(connfd, read_buf, buffer_size, 0)) > 0){
        bytes_read += n;
        if(bytes_read == 8388608)
            break;
    }

    printf("Sending Data :%d, bytes_read = %d\n", send(connfd, "1", 1, 0), bytes_read);
    }

    close(connfd);

    return 0;
}
