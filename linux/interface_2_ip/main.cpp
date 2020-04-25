#include <arpa/inet.h>
#include <sys/socket.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ifaddrs.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("usage: app <interface>\n");
        return EXIT_FAILURE;
    }

    struct ifaddrs *ifaddr, *ifa;
    char hbuf[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    /* Walk through linked list, maintaining head pointer so we can free list later */

    printf("IP address for: %s\n", argv[1]);
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == nullptr or 0 != strcmp(ifa->ifa_name, argv[1]))
        {
            continue;
        }

        int const family = ifa->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6)
        {
            int const s = getnameinfo(ifa->ifa_addr,
                    (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                    hbuf, sizeof hbuf,
                    nullptr, 0,
                    NI_NUMERICHOST);

           if (s != 0)
           {
               printf("getnameinfo() failed: %s\n", gai_strerror(s));
               exit(EXIT_FAILURE);
           }

           printf("%s (%d):\t <%s>\n", ((family == AF_INET) ? "AF_INET" : "AF_INET6"), family, hbuf);
       }
    }

    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
}
