#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

void child(int socket, int buffer_size) {
    char* read_buf;
    char* send_buf;
    char* temp_buf;
    int n, bytes_read=0, loops;
    double elaspedtime, minimum_elasped_time;
    struct timespec t1, t2;

    read_buf = (char *)calloc(buffer_size, sizeof(char));
    send_buf = (char *)calloc(buffer_size, sizeof(char));

    srand(time(NULL));

    temp_buf = send_buf;
    for (n = 0; n < buffer_size - 1; n++)
    {
        *temp_buf = 'A' + (random() % 26);
        temp_buf++;
    }

    for (loops = 0; loops <= 1000; loops++) {
        bytes_read = 0;

        while((n = read(socket, read_buf, buffer_size)) > 0)
        {
            bytes_read += n;
            if (bytes_read == buffer_size)
                break;
        }
        write(socket, send_buf, buffer_size);
    }
}

void parent(int socket, int buffer_size) {
    char* read_buf;
    char* send_buf;
    char* temp_buf;
    int n, bytes_read=0, loops;
    double elaspedtime, minimum_elasped_time;
    struct timespec t1, t2;

    read_buf = (char *)calloc(buffer_size, sizeof(char));
    send_buf = (char *)calloc(buffer_size, sizeof(char));
 
    srand(time(NULL));

    temp_buf = send_buf;
    for (n = 0; n < buffer_size - 1; n++)
    {
        *temp_buf = 'A' + (random() % 26);
        temp_buf++;
    }


    minimum_elasped_time = 1000000000;
    for (loops = 0; loops <= 1000; loops++) {

        bytes_read = 0;
        clock_gettime(CLOCK_MONOTONIC, &t1);

        write(socket, send_buf, buffer_size);

        while((n = read(socket, read_buf, buffer_size)) > 0)
        {
            bytes_read += n;
            if(bytes_read == buffer_size)
                break;
        }

        clock_gettime(CLOCK_MONOTONIC, &t2);
        elaspedtime = (t2.tv_sec - t1.tv_sec)*1000000000;
        elaspedtime += (t2.tv_nsec - t1.tv_nsec);

        if (elaspedtime < minimum_elasped_time)
            minimum_elasped_time = elaspedtime;

    }

    printf("Latency = %lf\n", minimum_elasped_time);

}

void main(int argc, char* argv[]) {
    int fd[2];
    static const int parentsocket = 0;
    static const int childsocket = 1;
    pid_t pid;
    int buffer_size = atoi(argv[1]); 

    socketpair(PF_LOCAL, SOCK_STREAM, 0, fd);

    pid = fork();
    if (pid == 0) {
        close(fd[parentsocket]);
        child(fd[childsocket], buffer_size);
    } else {
        close(fd[childsocket]);
        parent(fd[parentsocket], buffer_size);
    }

    exit(0);
}
