#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "log.h"
#include "argpars.h"


argpars parser[] = {
    {'h', "help", "Print this help message.", "help"},
    {'H', "host", "The target host name.", "string"},
    {'t', "timeout", "The timeout to wait per request.", "int", "1"},
    {'d', "debug", "Turn ON debugging.", "bool", "0"},
    {'T', "threads", "Number of threads to use (default is 10)", "int", "10"}
};

int check_port(char *host, unsigned port) {
    debug("Trying port %d in %s\r", port, host);
    int return_val;

    int net_socket;
    int conn_socket;
    struct sockaddr_in hostaddr;
    char *IPbuffer;
    struct hostent *host_entry;

    if ((net_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_exit("Could not create a socket!\n");

    bzero(&hostaddr, sizeof(hostaddr));
    hostaddr.sin_family = AF_INET;
    hostaddr.sin_port = htons(port);

    host_entry = gethostbyname(host);

    if ((IPbuffer = inet_ntoa(*((struct in_addr *) host_entry->h_addr_list[0]))) == NULL)
        err_exit("Could not get the host ip address!\n");

    if (inet_pton(AF_INET, IPbuffer, &hostaddr.sin_addr.s_addr) <= 0)
        err_exit("Could not convert the ip address to a binary ip address!\n");

    if ((conn_socket = connect(net_socket, (struct sockaddr *) &hostaddr, sizeof(hostaddr))) < 0)
        return_val = false;

    return_val = true;
    close(conn_socket);
    close(net_socket);

    return return_val;
}


int main(int argc, char *argv[]) {
    argpars *args = argpars_setup_env(argc, argv, parser, ARRAY_SIZE(parser));

    /*int timeout = atoi(get_value(args, ARRAY_SIZE(parser), "timeout");)*/

    int timeout = atoi(get_arg_value(args, ARRAY_SIZE(parser), "timeout")); // args_size eqaul to parser_size
    int debug = atoi(get_arg_value(args, ARRAY_SIZE(parser), "debug"));
    if (debug) allow_debug = true;
    char *host = get_arg_value(args, ARRAY_SIZE(parser), "host");
    int threads_n = atoi(get_arg_value(args, ARRAY_SIZE(parser), "threads"));

    unsigned working_ports[65535]; // I know that is sooooo mush big, but just to make space for all ports
    unsigned working_ports_i = 0;
    for (unsigned ports_i = 0; ports_i < 65535; ports_i++) {
        if (check_port(host, ports_i)) {
            working_ports[working_ports_i] = ports_i;
            info("%d is working!", ports_i);
        }
    }
    return 0;
}