#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "functions.h"

void print_usage(char * argv[]){
    printf("Usage: %s [-t <TARGET>] [-p <PORT>]\n",argv[0]);
}

void print_help(char * argv[]){
    printf("Artisan - TCP Port Scanner\n");
    printf("%s [-h], [--help]: Prints this.\n", argv[0]);
    printf("%s [-t <TARGET>] [-p <PORT>]: Scans selected TCP port.\n", argv[0]);
    printf("%s [-t <TARGET>] [-p <START-END>] [--range]: Scans selected range of TCP ports.\n", argv[0]);
}

int send_request(int clientSocket, int port){
    char request[256] = {0};
    char response[1024] = {0};

    // HTTP    
    if (port == 80){
        strncpy(request,"GET / HTTP/1.1\r\n\r\n", sizeof(request));
        
        printf("[SEND]: %d\n", port);
        send(clientSocket, request, sizeof(request), 0);
        if (errno){
            perror("Send");
            return -1;
        }
        recv(clientSocket, &response, sizeof(response), 0);
        if (errno){
            perror("Recv");
            return -1;
        }

        printf("[SERVER]: %s\n", response);
        close(clientSocket);
        return 0;
    } else {
        strncpy(request, "hello", sizeof(request));

        printf("[SEND]: %d\n", port);
        send(clientSocket, request, sizeof(request), 0);
        if (errno){
            perror("Send");
            return -1;
        }
        recv(clientSocket, &response, sizeof(response), 0);
        if (errno){
            perror("Recv");
            return -1;
        }

        printf("[SERVER]: %s\n", response);
        close(clientSocket);
    }
}

int build_sock_struct(int * clientSocketOut, struct sockaddr_in * AddressOut, int port, char * address){
    *clientSocketOut = socket(AF_INET, SOCK_STREAM, 0);
    if (*clientSocketOut == -1){
        perror("Socket");
        return -1;
    }

    struct sockaddr_in serverAddress = {0};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, address, &(serverAddress.sin_addr));

    //*clientSocketOut = sockfd;
    *AddressOut = serverAddress;

    return 0;
}

void sleep_ms (int ms){
    usleep(ms * 1000);
}

int * port_scan(int sockfd, struct sockaddr_in serverAddress, int firstPort, int lastPort, int * sizeOut){
    serverAddress.sin_port = htons(firstPort);
    int * openPorts = calloc(1, sizeof(int));
    if (openPorts == NULL){
        printf("The allocator failed.\n");
    }
    int j = 0;
    int conn = 0;

    if (lastPort > 0){
        for (int i = firstPort; i <= lastPort; i++){
            serverAddress.sin_port = htons(i);
            conn = connect(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
            if (conn == -1){
                printf("[CLOSED/FILTERED]: %d\n", i);
                errno = 0;
            } else {
                if (j > 0){
                    int *temp = openPorts;
                    openPorts = realloc(openPorts, (j+1) * sizeof(int));
                    if (!openPorts){
                        printf("The reallocation unexpectedly failed.\n");
                        openPorts = temp;
                    } else {
                        free(temp);
                    }
                }
                openPorts[j] = i;
                j++;
                printf("[OPEN]: %d\n", i);
            }
            // Sleep Here
            sleep_ms(10);
        }
    } else {
        conn = connect(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        if (conn == -1){
            printf("[CLOSED/FILTERED]: %d\n", firstPort);
            errno = 0;
        } else {
            openPorts[j] = firstPort;
            j++;
            printf("[OPEN]: %d\n", firstPort);
        }
    }
    *sizeOut = j;
    return openPorts;
}
