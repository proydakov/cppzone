#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    (void)(argc);
    (void)(argv);

    // port to start the server on
    unsigned short SERVER_PORT = 8877;

    // socket address used for the server
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    // htons: host to network short: transforms a value in host byte
    // ordering format to a short value in network byte ordering format
    server_address.sin_port = htons(SERVER_PORT);

    // htons: host to network long: same as htons but to long
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // create a UDP socket, creation returns -1 on failure
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("could not create socket\n");
        return 1;
    }

    // bind it to listen to the incoming connections on the created server
    // address, will return -1 on error
    if ((bind(sock, (struct sockaddr *)&server_address, sizeof(server_address))) < 0) {
        printf("could not bind socket\n");
        return 1;
    }

    // socket address used to store client address
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    // run indefinitely
    while (true) {
        char buffer[1024];

        // read content into buffer from an incoming client
        ssize_t const len = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &client_address_len);

        if (len < 0)
        {
            continue;
        }

        size_t const clen = (size_t)(len);

        // send same content back to the client ("echo")
        sendto(sock, buffer, clen, 0, (struct sockaddr*)&client_address, sizeof(client_address));

        buffer[len] = '\0';
        // inet_ntoa prints user friendly representation of the ip address
        printf("received: '%s' from client %s:%d\n", buffer, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    }

    return 0;
}
