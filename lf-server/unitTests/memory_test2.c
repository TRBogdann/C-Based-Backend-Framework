#include "../lf_http.h"
#include <malloc.h>


void test1()
{
    Tokener t1 = createTokener("./requestTests/req1.http",LF_FILE_MODE);
    Tokener t2 = createTokener("./requestTests/req2.http",LF_FILE_MODE);
    Tokener t3 = createTokener("./requestTests/req3.http",LF_FILE_MODE);
    
    HttpObject object1 = newHttpObject(&t1,5,1024);
    HttpObject object2 = newHttpObject(&t2,5,1024);
    HttpObject object3 = newHttpObject(&t3,5,1024);

    char* message = HttpObjectToString(object2);

    free(message);

    deleteTokener(&t1);
    deleteTokener(&t2);
    deleteTokener(&t3);

    deleteHttpObject(&object1);
    deleteHttpObject(&object2);
    deleteHttpObject(&object3);
}


void test2()
{
    Tokener t1 = createTokener("./requestTests/req1.http",LF_FILE_MODE);
    Tokener t2 = createTokener("./requestTests/req2.http",LF_FILE_MODE);
    Tokener t3 = createTokener("./requestTests/req3.http",LF_FILE_MODE);
    
    HttpRequest object1 = newHttpRequest(&t1,5,1024);
    HttpRequest object2 = newHttpRequest(&t2,5,1024);
    HttpRequest object3 = newHttpRequest(&t3,5,1024);
    
    char* message = HttpRequestToString(object2);

    free(message);

    deleteTokener(&t1);
    deleteTokener(&t2);
    deleteTokener(&t3);

    deleteHttpRequest(&object1);
    deleteHttpRequest(&object2);
    deleteHttpRequest(&object3);
}

void test3()
{
 HttpResponse response = newHttpResponse("Hello World",status_200,5);
 
 addEntryTable(response.header,newEntry("Content-Type","plain-text"));
 addEntryTable(response.header,newEntry("Content-Lenght","12"));
 
 char* message = HttpResponseToString(response);

 free(message);
 deleteHttpResponse(&response);
};

int main()
{
    while(1)
    {
        test1();
        test2();
        test3();
    }
    return 0;
}