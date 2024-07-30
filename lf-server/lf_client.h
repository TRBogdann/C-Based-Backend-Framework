#include <netinet/in.h>

typedef struct LfClient
{
    int clientId; 
    struct sockaddr_in data;
}LfClient;