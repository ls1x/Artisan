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

    static struct option long_options[] = {
    { "help", 0, NULL, 'h' },
    { "range", 2, NULL, 'r' },
    { 0, 0, 0, 0}};


    while ((c = getopt_long(argc,argv,"ht:p:",long_options, NULL)) != -1){
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
        char *endptr = NULL;
        
        // Socket Structure
        int clientSocket = 0;
        struct sockaddr_in serverAddress = {0};
        int buildstruct = build_sock_struct(&clientSocket, &serverAddress, port[0], addressC);
        if (buildstruct == -1){
            printf("Something went wrong when building the socket structure.\n");
            return -1;
        }
        
        if (isPortRange == true){
            errno = 0;
            port[0] = strtol(strtok(portC,"-"), &endptr, 10);
            if (errno != 0){
                printf("Something went wrong when converting the port from string to int.\n");
                perror("Strtol");
                return -1;
            }
            port[1] = strtol(strtok(NULL,"-"), &endptr, 10);
            if (errno != 0){
                printf("Something went wrong when converting the port from string to int.\n");
            }
            // Scan the range of ports
            openPorts = port_scan(clientSocket, serverAddress, port[0], port[1], &sizeOut);
        
        } else {
            port[0] = strtol(strtok(portC,"-"), &endptr, 10);
            if (errno != 0){
                printf("Something went wrong when converting the port from string to int.\n");
                perror("Strtol");
                return -1;
            }
            openPorts = port_scan(clientSocket, serverAddress, port[0], port[1], &sizeOut);
        }
        
        /* Tasks:
         * - [ ] Ainda dá connection refused, mesmo a porta aparecendo como open
         * - [x] FIXED: Memory leaks
         */

        sleep(3);
        for (int i = 0; i < sizeOut; i++){
            int request = send_request(clientSocket, openPorts[i]);
            if (request == -1){
                // send_request should be void.
                // perror is not needed here
            }
        }
        
        // Cleanup
        close(clientSocket);
        free(openPorts);

    } else {
        printf("IP Address and Port are required values.\n");
        print_usage(argv);
        return 0;
    }
   
    return 0;
}
