#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "functions.h"

int main(int argc, char * argv[]){
    int c = 0;
    char *addressC = NULL;
    char *portC;
    bool isPortRange = false;
    bool isVerbose = false;

    static struct option long_options[] = {
    { "help", 0, NULL, 'h' },
    { "range", 2, NULL, 'r' },
    { 0, 0, 0, 0}};


    while ((c = getopt_long(argc,argv,"ht:p:v",long_options, NULL)) != -1){
        switch (c){
            case 'h':
                print_help(argv);
                return 0;
            case 't':
                addressC = optarg;
                break;
            case 'p':
                portC = optarg;
                break;
            case 'r':
                isPortRange = true;
                break;
            case 'v':
                isVerbose = true;
            case '?':
                break;

            default:
                return -1;    
        }
    }

    if (addressC && portC){
        int port[2] = {0};
        int *openPorts = NULL;
        int sizeOut = 0;
        char * strtoked_port = NULL;
        char *endptr = NULL;
        
        if (isPortRange == true){
            errno = 0;
            strtoked_port = strtok(portC,"-");
            if (strtoked_port == NULL){
                printf("[ERR]: Incorrectly typed range of ports.\n");
                return -1;
            }
            port[0] = strtol(strtoked_port, &endptr, 10);
            if (errno != 0){
                printf("[ERR]: Something went wrong when converting the port from string to int.\n");
                perror("Strtol");
                return -1;
            }
            strtoked_port = strtok(NULL,"-");
            if (strtoked_port == NULL){
                printf("[ERR]: Incorrectly typed range of ports.\n");
                return -1;
            }
            port[1] = strtol(strtoked_port, &endptr, 10);
            if (errno != 0){
                printf("[ERR]: Something went wrong when converting the port from string to int.\n");
            }
            if (port[0] > 65535 || port[1] > 65535){
                printf("[ERR]: Ports cannot be higher than 65535.\n");
                return -1;
            }
            // Scan the range of ports
            openPorts = port_scan(addressC, port[0], port[1], &sizeOut, isVerbose);
        } else {
            port[0] = strtol(portC, &endptr, 10);
            if (errno != 0){
                printf("[ERR]: Something went wrong when converting the port from string to int.\n");
                perror("Strtol");
                return -1;
            }
            // Scan a single port
            openPorts = port_scan(addressC, port[0], port[1], &sizeOut, isVerbose);
        }
        
        // Sending Requests
        for (int i = 0; i < sizeOut; i++){
            int request = 0;
            request = send_request(addressC, openPorts[i]);
        }
   
        // Cleanup
        free(openPorts);

    } else {
        printf("[ERR]: IP Address and Port are required values.\n");
        print_usage(argv);
        return 0;
    }
   
    return 0;
}
