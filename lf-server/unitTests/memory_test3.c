#include "../lf_http.h"
#include <malloc.h>

void test1()
{
    Tokener tokener1 = createTokener("./requestTests/req4.http",LF_FILE_MODE);
    HttpRequest request = newHttpRequest(&tokener1,5,1024);
    JSONObject obj = JSONParseRequest(request,5,1024);

    deleteTokener(&tokener1);
    deleteTable(&obj);
    deleteHttpRequest(&request);
}

void test2()
{
    HttpResponse response = HTMLResponse("./requestTests/req5.html",status_200,5,1024);
    char* message = HttpResponseToString(response);

    free(message);
    deleteHttpResponse(&response);
}

int main()
{
    while(1)
    {
        test1();
        test2();
    }
    return 0;
}