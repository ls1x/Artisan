#ifndef PORTSCANNER_H
#define PORTSCANNER_H

// Insert Functions here
void print_usage(char * argv[]);
void print_help(char * argv[]);
int build_sock_struct(int * clientSocketOut, struct sockaddr_in * AddressOut, int port, char * address);
int * port_scan(int sockfd, struct sockaddr_in serverAddress, int firstPort, int lastPort, int * sizeOut);
int send_request(int clientSocket, int port);

#endif

