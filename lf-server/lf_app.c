#include "lf_app.h"
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>


char AppHandler(LfApp* this,HttpRequest* request,LfClient* client)
{
    
    if(strcmp(request->path,"/")==0 || strcmp(request->path,"")==0)
    {
        if(this->routes!=NULL && this->routes->handle!=NULL){
            this->routes->handle(*client,*request);
             return LF_HANDLED;
        }  
    }

    LfRoute* route = lfSearchRoute(this->routes,request->path);
    if(route!=NULL && route->handle!=NULL)
    {
        route->handle(*client,*request);
        return LF_HANDLED;
    }

    return LF_UNHANDLED;
}

LfApp lfNewApp(int bufferSize)
{
    LfApp app;

    app.settings.allowedOrigins = NULL;
    app.settings.resources = NULL;
    app.settings.useCorsHeaders = 0;
    app.settings.domain = LF_IPV4;
    app.settings.socketType = LF_TCP;
    app.settings.connectionsLimit = 500;
    app.settings.timeOut = 10;

    app.handler = AppHandler;
    app.activeConnections = 0;
    app.bufferSize = bufferSize;
    app.closed = 0;
    app.routes = newLfRoute("");
    //app.routes = NULL;

    return app;
}

char lfRequestChecker(HttpRequest request, LfClient client)
{
    char checker = checkRequest(request);
    if(checker==http_invalid || checker==http_wrong_format)
    {
        send(client.clientId, "HTTP/1.1 400 Bad Request", strlen("HTTP/1.1 400 Bad Request"), 0);
        return LF_HANDLED;
    }
    return LF_UNHANDLED;
}

void* lfDefaultAppHandler(void* arg)
{
    LfWorker* worker = (LfWorker*)arg;

    worker->app->activeConnections++;
    

    char* requestText = (char*)malloc(worker->app->bufferSize+10);
    int messageLen = read(worker->client->clientId, requestText, worker->app->bufferSize+9);
    
    
    if(messageLen > 0 && worker->app->bufferSize>messageLen) {
            
    requestText[messageLen] = '\0'; // Null-terminate the string
    printf("%d %d\n",worker->app->bufferSize,messageLen);
   
    printf("New request:%s\n",requestText);
    Tokener t = createTokener(requestText,LF_TEXT_MODE);
    
    HttpRequest request = newHttpRequest(&t,20,worker->app->bufferSize+10);
   
    if(lfRequestChecker(request,*worker->client)!=LF_HANDLED)
      { 
        char handled = worker->app->handler(worker->app,&request,worker->client);
        if(handled == LF_UNHANDLED){
            send(worker->client->clientId, "HTTP/1.1 404 Not Found", strlen("HTTP/1.1 404 Not Found"), 0);
        }
    }

    deleteHttpRequest(&request);
    deleteTokener(&t);
    }
     
    else if(messageLen <= 0)
        send(worker->client->clientId, "HTTP/1.1 400 Bad Request", strlen("HTTP/1.1 400 Bad Request"), 0);
    
    else 
        send(worker->client->clientId, "HTTP/1.1 500 Internal Server Error", strlen("HTTP/1.1 500 Internal Server Error"), 0);
 
    free(requestText);
    close(worker->client->clientId); 
    worker->app->activeConnections--;
    pthread_exit(NULL);
    
    return NULL;
}

LfWorker lfCreateWorker(LfApp* app,LfClient* client)
{
    LfWorker worker;
    worker.app = app;
    worker.client = client;
    return worker;
}

void lfStartWorker(LfWorker* worker)
{
    pthread_create(&worker->workerId, NULL, lfDefaultAppHandler, worker);
    pthread_detach(worker->workerId);
}

void sigpipe_handler(int unused)
{
    fprintf(stderr,"[Warning]: A client-sever connection was interupted\n");
}

void lfStartApp(LfApp app,int port)
{
    signal(SIGPIPE, sigpipe_handler);

    //1. FILE DESCRIPTOR
    int serverId = socket(app.settings.domain,app.settings.socketType,0);

    if(serverId==-1)
    {
        fprintf(stderr,"[ERROR]: Could not create a file descriptor for the socket\n");
        return;
    }

    //2.Socket informations 
    struct sockaddr_in serverSocket;
    serverSocket.sin_port = htons(port);
    serverSocket.sin_family = app.settings.domain;
    serverSocket.sin_addr.s_addr = INADDR_ANY;

    //3.Socket Options
    
    int checkopt1;
    int checkopt2 = 1;
    int checkopt3 = 1;


    
    int opt = 1;
    checkopt1 = setsockopt(serverId, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    
    if(app.settings.timeOut>0)
    {
    struct timeval timeout;      
    timeout.tv_sec = app.settings.timeOut;
    timeout.tv_usec = 0;

    checkopt2=setsockopt(serverId, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    checkopt3=setsockopt(serverId, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    }

    if(checkopt1==-1 || checkopt2==-1 || checkopt3==-1)
    {
        fprintf(stderr,"[ERROR]: Could not set options for the socket\n");
        return;

    }

    //4.Bind
    int checkBind = bind(serverId, (const struct sockaddr*)&serverSocket, sizeof(serverSocket));
    if(checkBind == -1)
    {
        fprintf(stderr,"[ERROR]: Could not bind socket\n");
        return;
    }

    //5.Listen
    int checkListen = listen(serverId,app.settings.connectionsLimit);
    
    if(checkListen==-1)
    {
        fprintf(stderr,"[ERROR]: Could not listen to the port\n");
    }

    while(!app.closed)
    {   
        LfClient client;
        socklen_t len = sizeof(client.data);
        client.clientId = accept(serverId,(struct sockaddr*)&client.data,&len);

        if(client.clientId==-1)
        {
            //fprintf(stderr,"[Warning]: Connection TimeOut\n");
        }
        else 
        {

        if(app.activeConnections>=app.settings.connectionsLimit)
        {
            int checkSend = send(client.clientId, "HTTP/1.1 503 Service Unavailable", strlen("HTTP/1.1 503 Service Unavailable"), 0);;
            close(client.clientId);
        }
        else
        {
            LfWorker worker = lfCreateWorker(&app, &client);
            lfStartWorker(&worker);
        }
        }
    }
    
}

void lfAppAddRoute(LfApp* app,LfRoute* route)
{
    lfAddSubRoute(&app->routes,route);
}
void LfExitApp(LfApp* app)
{
    app->closed = 1;
}

void lfDeleteApp(LfApp* app)
{
    deleteLfRoute(&app->routes);
    app->handler = NULL;
    
    if(app->settings.allowedOrigins!=NULL)
    {
        free(app->settings.allowedOrigins);
        app->settings.allowedOrigins = NULL;
    }

    if(app->settings.resources!=NULL)
    {
        free(app->settings.resources);
        app->settings.resources = NULL;
    }
}   


void lfSetMainRouteHandler(LfApp *app,void (*f)(LfClient client,HttpRequest req))
{

}

void lfSetConnectionLimit(LfApp *app,int connectionsLimit)
{

}

void lfUseCors(LfApp* app,char value)
{

}

void lfSetConnectionTimeout(LfApp* app,int timeout)
{

}

void lfSetAllowedOrigins(LfApp* app,char** origins,int size)
{

}

void lfSetResources(LfApp* app,char** origins,int size)
{

}

void lfUseConfigFile(LfApp *app,const char* jsonFile)
{

}

void lfSetSocketType(LfApp* app,int socketType)
{

}

void lfSetInetProtocol(LfApp* app,int inetProtocol)
{

}