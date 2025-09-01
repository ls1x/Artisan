#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "functions.h"

int main(int argc, char * argv[]){
    int c = 0;
    char *addressC = NULL;
    char *portC = NULL;

    while ((c = getopt(argc,argv,"h:p:")) != -1){
        switch (c){
            case 'h':
                addressC = optarg;
                break;
            case 'p':
                portC = optarg;
                break;
            case '?':
                break;

            default:
                return -1;    
        }
    }

    if (addressC && portC){
        int port = 0;
        char *endptr = NULL;
        
        errno = 0;
        port = strtol(portC, &endptr, 10);
        if (errno != 0){
            printf("Something went wrong when converting the port from string to int");
            perror("strtol");
            return -1;
        }

        // Socket and Address
        int clientSocket = 0;
        clientSocket = socket(AF_INET,SOCK_STREAM, 0);

        // Remote Address
        struct sockaddr_in serverAddress = {0};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        inet_pton(AF_INET, addressC, &(serverAddress.sin_addr));

        int conn = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        if(conn == -1){
            printf("Something went wrong when trying to connect.\n");
            perror("perror");
            return -1;
        }
        
        // For testing purposes: HTTP
        char request[] = "GET / HTTP/1.1\r\n\r\n"; //Change this
        char response[4096] = {0};

        send(clientSocket, request, sizeof(request), 0);
        recv(clientSocket, &response, sizeof(response), 0);

        printf("[SERVER]: %s\n", response);
        close(clientSocket);

    } else {
        printf("IP Address and Port are required values.\n");
        print_usage(argv);
        return 0;
    }
    
    return 0;
}
