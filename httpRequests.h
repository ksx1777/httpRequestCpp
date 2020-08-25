#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/* funções declaradas aqui */
int createSockets(char host[], char *port);
int sendRequests(char host[], char *port, char urlPath[]);
