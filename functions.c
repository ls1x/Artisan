#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "functions.h"

void print_usage(char * argv[]){
    printf("Usage: %s -h <HOST> -p <PORT>\n",argv[0]);
}
