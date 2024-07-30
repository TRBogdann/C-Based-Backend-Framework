#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "lf-server/lf_routes.h"
#include <malloc.h>

void handleMain(LfClient client,HttpRequest request)
{
    if(strcmp(request.method,"GET")==0)
    {
        HttpResponse response = HTMLResponse("testIndex/indexmain.html",status_200,5,1024);
        char* message = HttpResponseToString(response);
        send(client.clientId, message, strlen(message), 0);
        free(message);
        deleteHttpResponse(&response);
    }
    else 
        lfDefaultHandler(client,request);
}

void handleProj(LfClient client,HttpRequest request)
{
    if(strcmp(request.method,"GET")==0)
    {
        HttpResponse response = HTMLResponse("testIndex/indexproj.html",status_200,5,1024);
        char* message = HttpResponseToString(response);
        send(client.clientId, message, strlen(message), 0);
        free(message);
        deleteHttpResponse(&response);
    }
    else 
        lfDefaultHandler(client,request);
}

void handleAsset(LfClient client,HttpRequest request)
{
    if(strcmp(request.method,"GET")==0)
    {
        HttpResponse response = HTMLResponse("testIndex/indexasset.html",status_200,5,1024);
        char* message = HttpResponseToString(response);
        send(client.clientId, message, strlen(message), 0);
        free(message);
        deleteHttpResponse(&response);
    }
    else 
        lfDefaultHandler(client,request);
}

void switchRoutes(LfRoute* route,LfClient client,HttpRequest request)
{
    char checker = checkRequest(request);
    if(checker==http_invalid || checker==http_wrong_format)
    {
        send(client.clientId, "HTTP/1.1 400 Bad Request", strlen("HTTP/1.1 400 Bad Request"), 0);
        return;
    }
    if(strcmp(request.path,"/")==0 && strcmp(request.method,"GET")==0)
    {
        HttpResponse response = HTMLResponse("lf-server/unitTests/requestTests/req5.html",status_200,5,1024);
        char* message = HttpResponseToString(response);
        send(client.clientId, message, strlen(message), 0);
        free(message);
        deleteHttpResponse(&response);
        return;
    }
    
    if(strcmp(request.path,"/main")==0 && strcmp(request.method,"GET")==0)
    {
        route->handle(client,request);
        return;
    }
    LfRoute* wanted = lfSearchRoute(route,request.path+5); 
    if(wanted==NULL)
    {
        send(client.clientId, "HTTP/1.1 404 Not Found", strlen("HTTP/1.1 404 Not Found"), 0);
        return;;
    }

    wanted->handle(client,request);
}

LfRoute* init()
{
    LfRoute* route = newLfRoute("main");
    
    lfAddSubRoute(&route, newLfRoute("proj1"));
    lfAddSubRoute(&route, newLfRoute("proj2"));
    lfAddSubRoute(&route->subRoutes, newLfRoute("asset1"));
    lfAddSubRoute(&route->subRoutes->next, newLfRoute("asset1"));
    lfAddSubRoute(&route->subRoutes, newLfRoute("asset2"));
    lfAddSubRoute(&route->subRoutes->next, newLfRoute("asset2"));

   lfAddHandlerRoute(route,handleMain);
   lfAddHandlerRoute(lfSearchRoute(route,"proj1"),handleProj);
   lfAddHandlerRoute(lfSearchRoute(route,"proj2"),handleProj);
   lfAddHandlerRoute(lfSearchRoute(route,"proj1/asset1"),handleAsset);
   lfAddHandlerRoute(lfSearchRoute(route,"proj2/asset1"),handleAsset);
   lfAddHandlerRoute(lfSearchRoute(route,"proj1/asset2"),handleAsset);
   lfAddHandlerRoute(lfSearchRoute(route,"proj2/asset2"),handleAsset);

    return route;
}

int main() {
    int serverId = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverSocket;
    serverSocket.sin_port = htons(2020);
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_addr.s_addr = INADDR_ANY;
    int opt = 1;

    setsockopt(serverId, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    int stat = bind(serverId, (const struct sockaddr*)&serverSocket, sizeof(serverSocket));
    
    listen(serverId, 5);

    char request[1024];
 

    char closed = 0;
    socklen_t len = sizeof(serverSocket);

    LfRoute* route = init();

       while (!closed) {
        int clientId = accept(serverId, (struct sockaddr*)&serverSocket, &len);
        int messageLen = read(clientId, request, 1023);
        if (messageLen > 0) {
            
            request[messageLen] = '\0'; // Null-terminate the string
                    char client_ip[INET_ADDRSTRLEN];
            
            inet_ntop(AF_INET, &(serverSocket.sin_addr), client_ip, INET_ADDRSTRLEN);
            printf("Connection accepted from %s:%d\n", client_ip, ntohs(serverSocket.sin_port));

            printf("New request:\n %s\n", request);
            
            Tokener t = createTokener(request,LF_TEXT_MODE);
            HttpRequest request = newHttpRequest(&t,5,1024);
            LfClient client = (LfClient){clientId,serverSocket};
            
            switchRoutes(route,client,request);
            
            deleteHttpRequest(&request);
            deleteTokener(&t);
        }
        
        close(clientId);
    }

    deleteLfRoute(&route);
    close(serverId);
    return 0;
}