#include "lf_routes.h"

#define LF_IPV4 AF_INET
#define LF_IPV6 AF_INET6
#define LF_TCP SOCK_STREAM
#define LF_UDP SOCK_DGRAM 

#define LF_HANDLED 1
#define LF_UNHANDLED 0

// domain AF_INET IPv4
// domain AF_INET6 IPv6

typedef struct LfAppSettings
{
    int timeOut;
    int connectionsLimit;
    int socketType;
    int domain;
    char** allowedOrigins; //NULL
    char** resources; //NULL
    char useCorsHeaders;
}LfAppSettings;

typedef struct LfApp{
    int activeConnections;
    char (*handler)(struct LfApp* app,HttpRequest* request,LfClient* client);
    int bufferSize;
    char closed;
    LfRoute* routes;
    LfAppSettings settings;
}LfApp;

typedef struct LfWorker
{
    pthread_t workerId;
    LfApp* app;
    LfClient* client;
}LfWorker;

LfWorker lfCreateWorker(LfApp* app,LfClient* client);
void lfStartWorker(LfWorker* worker);
void lfDeleteWorker(LfWorker* worker);

//To-Do: Creaza un template 
void* lfDefaultAppHandler(void*);
char lfRequestChecker(HttpRequest request, LfClient client);
LfApp lfNewApp(int bufferSize);
void lfStartApp(LfApp app,int port);


//Neterminate
void lfAppAddRoute(LfApp *app,LfRoute *route);
void LfExitApp(LfApp* app);
void lfDeleteApp(LfApp* app);

//+setteri
void lfSetMainRouteHandler(LfApp *app,void (*f)(LfClient client,HttpRequest req));
void lfSetConnectionLimit(LfApp *app,int connectionsLimit);
void lfUseCors(LfApp* app,char value);
void lfSetConnectionTimeout(LfApp* app,int timeout);
void lfSetAllowedOrigins(LfApp* app,char** origins,int size);
void lfSetResources(LfApp* app,char** origins,int size);
void lfUseConfigFile(LfApp *app,const char* jsonFile);
void lfSetSocketType(LfApp* app,int socketType);
void lfSetInetProtocol(LfApp* app,int inetProtocol);
