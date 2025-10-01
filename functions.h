#ifndef PORTSCANNER_H
#define PORTSCANNER_H

// Insert Functions here
void print_usage(char * argv[]);
void print_help(char * argv[]);
int build_sock_struct(int * clientSocketOut, struct sockaddr_in * AddressOut, int port, char * address);
int * port_scan(char * serverAddress_char, int firstPort, int lastPort, int * sizeOut, bool isVerbose, int sleepMs); 
int send_request(char * serverAddress_char, int port);

#endif

