#include "lf_client.h"
#include "lf_http.h"

typedef struct LfRoute
{
    struct LfRoute* next;
    struct LfRoute* subRoutes;
    char* name;
    void (*handle)(LfClient client,HttpRequest req);
}LfRoute;

void lfDefaultHandler(LfClient c,HttpRequest req);
LfRoute* newLfRoute(const char* name);
void deleteLfRoute(LfRoute** route);
void printLfRoute(LfRoute* route,int level);
void lfAddSubRoute(LfRoute** route,LfRoute *subRoute);
void lfAddHandlerRoute(LfRoute* route,void (*f)(LfClient client,HttpRequest req));
LfRoute* lfGetSubRoute(LfRoute* route,const char* name);
LfRoute* lfSearchRoute(LfRoute* route,const char* fullRoute); 
