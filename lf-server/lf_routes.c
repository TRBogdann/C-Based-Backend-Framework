#include "lf_routes.h"
#include <malloc.h>
#include <string.h>
void lfDefaultHandler(LfClient client,HttpRequest req)
{
    send(client.clientId,"HTTP/1.1 405 Method Not Allowed",sizeof("HTTP/1.1 405 Method Not Allowed"),0);
}

LfRoute* newLfRoute(const char* name)
{
    LfRoute* route = (LfRoute*)malloc(sizeof(LfRoute));
    route->subRoutes = NULL;
    route->next = NULL;
    route->name = (char*)malloc(strlen(name)+1);
    strcpy(route->name,name); 
    return route;
}

void deleteLfRoute(LfRoute** route)
{
    if(*route==NULL)
        return;
    deleteLfRoute(&(*route)->subRoutes);
    deleteLfRoute(&(*route)->next);
    free((*route)->name);
    free(*route);
    *route = NULL;
}

void lfAddSubRoute(LfRoute** route,LfRoute *subRoute)
{
    if(*route==NULL)
        return;
    
    if((*route)->subRoutes==NULL)
    {
        (*route)->subRoutes = subRoute;
        return;
    }

    LfRoute* list = (*route)->subRoutes;
    while(list->next!=NULL)
        list = list->next;

    list->next = subRoute;
}

void printLfRoute(LfRoute* route,int level)
{
    
    if(route==NULL)
        return;
    
    for(int i = 0;i<level;i++)
    {
        printf(" ");
    }
    printf("/%s\n",route->name);
    printLfRoute(route->subRoutes,level+1);
    printLfRoute(route->next,level);
}

void lfAddHandlerRoute(LfRoute* route,void (*f)(LfClient client,HttpRequest req))
{
    if(route==NULL)
        return;
    route->handle = f;
}

LfRoute* lfGetSubRoute(LfRoute* route,const char* name)
{
    if(route==NULL)
        return NULL;

    LfRoute* list =route->subRoutes;
    while (list!=NULL) {
        if(strcmp(list->name,name)==0)
            return list;
        list = list->next;
    }
    return NULL;
}

LfRoute* lfSearchRoute(LfRoute* route,const char* fullRoute)
{
    if(route==NULL || fullRoute == NULL)
        return NULL;
    
    LfRoute* path = route;
    
    const char* temp = (char*)(fullRoute[0]=='/'?(fullRoute+1):fullRoute);
    char* str = (char*)malloc(sizeof(temp)+1);
    strcpy(str,temp);
    
    char* token = strtok(str,"/");
    while(token!=NULL)
    {   
        path = lfGetSubRoute(path,token);
        
        if(path==NULL)
        {
            free(str);
            return  NULL;
        }

        token = strtok(NULL,"/");
    }
    free(str);
   
    return path;
}