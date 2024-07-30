#include "../lf_http.h"
#include <malloc.h>

int main()
{

    //test1
    Tokener tokener1 = createTokener("./requestTests/req4.http",LF_FILE_MODE);
    HttpRequest request = newHttpRequest(&tokener1,5,1024);
    JSONObject obj = JSONParseRequest(request,5,1024);
    showAllEntries(obj);

    //test2
    HttpResponse response = HTMLResponse("./requestTests/req5.html",status_200,5,1024);
    char* message = HttpResponseToString(response);
    printf("%s\n",message);

    free(message);
    deleteHttpResponse(&response);
    deleteTokener(&tokener1);
    deleteTable(&obj);
    deleteHttpRequest(&request);
    return 0;
}