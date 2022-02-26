#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "argpars.h"
#include "log.h"

int main(int argc, char *argv[]) {
    int socketfd, try_port, ports_start, ports_end;
    struct sockaddr_in socket_addr;
    struct hostent *host;
    char *hostname = "127.0.0.1";

    bzero(&socket_addr, sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    
    if (isint_str(hostname)) {
    socket_addr.sin_addr.s_addr = inet_addr(hostname);
    }
    else if ((host = gethostbyname(hostname)) != 0) {
        inet_aton(host->h_addr, &socket_addr.sin_addr);
    }
    else {
        herror(hostname);
        exit(1);
    }

    for (int ports_i = ports_start; ports_i <= ports_end; ports_i++) {
        socket_addr.sin_port = htons(ports_i);
        
        if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) err_exit("Could not create a socket!");
        if ((try_port = connect(socketfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr))) < 0) continue;
        
        info("Port %d is working!", ports_i);
    }

    return 0;
}