
#include "../lf_http.h"
#include <stdio.h>
#include <malloc.h>

void testObject(HttpObject t1)
{
    if(t1.messageLine)
        printf("Message:%s\n",t1.messageLine);
    

    if(!isEmptyTable(t1.header))
    {
        printf("Header:\n");
        showAllEntries(t1.header);
    }

    if(t1.content)
        printf("Content:%s\n\n",t1.content);
}

void test1()
{
    Tokener t1 = createTokener("./requestTests/req1.http",LF_FILE_MODE);
    Tokener t2 = createTokener("./requestTests/req2.http",LF_FILE_MODE);
    Tokener t3 = createTokener("./requestTests/req3.http",LF_FILE_MODE);
    
    HttpObject object1 = newHttpObject(&t1,5,1024);
    HttpObject object2 = newHttpObject(&t2,5,1024);
    HttpObject object3 = newHttpObject(&t3,5,1024);
    
    testObject(object1);
    testObject(object2);
    testObject(object3);

    char* message = HttpObjectToString(object2);
    printf("%s\n",message);

    free(message);
    
    deleteTokener(&t1);
    deleteTokener(&t2);
    deleteTokener(&t3);

    deleteHttpObject(&object1);
    deleteHttpObject(&object2);
    deleteHttpObject(&object3);
}

void testRequest(HttpRequest request)
{
    if(request.method)
    {
        printf("Method:%s\n",request.method);
        printf("Path:%s\n",request.path);
        printf("Protocol:%s\n",request.protocol);
    }

    if(!isEmptyTable(request.header))
    {
        printf("Headers:\n");
        showAllEntries(request.header);
    }

    if(request.content)
    {
        printf("Content:%s\n\n",request.content);
    }

}

void test2()
{
    Tokener t1 = createTokener("./requestTests/req1.http",LF_FILE_MODE);
    Tokener t2 = createTokener("./requestTests/req2.http",LF_FILE_MODE);
    Tokener t3 = createTokener("./requestTests/req3.http",LF_FILE_MODE);
    
    HttpRequest object1 = newHttpRequest(&t1,5,1024);
    HttpRequest object2 = newHttpRequest(&t2,5,1024);
    HttpRequest object3 = newHttpRequest(&t3,5,1024);
    
    testRequest(object1);
    testRequest(object2);
    testRequest(object3);

    char* message = HttpRequestToString(object2);
    printf("%s\n",message);
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
 printf("%s\n",message);

 free(message);
 deleteHttpResponse(&response);
};

int main()
{
    //Status Macro Test
    printf("1.Status Definiton Test\n");
    printStatus(status_101);
    printStatus(status_206);
    printStatus(status_305);
    printStatus(status_404);

    printf("\n\n2.Http Objects Tests\n");
    printf("Test1:\n");
    test1();
    printf("Test2:\n");
    test2();
    printf("Test3:\n");
    test3();
    return 0;
}