#include "lf-server//lf_app.h"
#include <string.h>
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


void  init(LfApp* app)
{
    LfRoute* route = app->routes;
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

    app->routes = route;
}


int main()
{
    LfApp app = lfNewApp(1024);
    init(&app);
    lfStartApp(app,2020);
    lfDeleteApp(&app);

    return 0;
}
