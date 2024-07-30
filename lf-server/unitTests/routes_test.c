#include "../lf_routes.h"
#include "stdio.h"
int main()
{
    LfRoute* route = newLfRoute("main");
    
    lfAddSubRoute(&route, newLfRoute("proj1"));
    lfAddSubRoute(&route, newLfRoute("proj2"));
    lfAddSubRoute(&route->subRoutes, newLfRoute("asset1"));
    lfAddSubRoute(&route->subRoutes->next, newLfRoute("asset1"));
    lfAddSubRoute(&route->subRoutes, newLfRoute("asset2"));
    lfAddSubRoute(&route->subRoutes->next, newLfRoute("asset2"));
    
    if(lfSearchRoute(route,"/proj1/asset1")==NULL)
        printf("Not Found\n");
    else
        printf("%s\n",lfSearchRoute(route,"/proj1/asset1")->name);
    
    printLfRoute(route,0);
    deleteLfRoute(&route);
    return 0;;
}