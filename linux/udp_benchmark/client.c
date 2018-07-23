#include <time.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("usage: %s '<server_addr>' '<message>'\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char* server_name = argv[1];
    const int server_port = 8877;

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    // creates binary representation of server name
    // and stores it as sin_addr
    // http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
    inet_pton(AF_INET, server_name, &server_address.sin_addr);

    // htons: port in network order format
    server_address.sin_port = htons(server_port);

    // open socket
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("could not create socket\n");
        return 1;
    }

    // data that will be sent to the server
    const char* data_to_send = argv[2];

    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    unsigned long const start_micro = tp.tv_sec * 1000 * 1000 + tp.tv_nsec / 1000;

    // send data
    int const len = sendto(sock, data_to_send, strlen(data_to_send), 0,
            (struct sockaddr*)&server_address, sizeof(server_address));

    // received echoed data back
    char buffer[1024];
    recvfrom(sock, buffer, len, 0, NULL, NULL);

    clock_gettime(CLOCK_REALTIME, &tp);
    unsigned long const end_micro = tp.tv_sec * 1000 * 1000 + tp.tv_nsec / 1000;
    unsigned long const delta = (end_micro - start_micro) / 1000;

    buffer[len] = '\0';
    printf("time delta: %ld microseconds, recieved: '%s'\n", delta, buffer);

    // close the socket
    close(sock);
    return 0;
}
