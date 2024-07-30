#include "lf_http.h"
#include <stdlib.h>
#include <string.h>

void printStatus(HttpStatus status)
{
    printf("HTPP/1.1 %d %s\n",status.code,status.text);
}

char checkProtocol(char* str)
{
    if(str[0]!='H')
        return 0;
    if(str[1]!='T')
        return 0;
    if(str[2]!='T')
        return 0;
    if(str[3]!='P')
        return 0;

    return 1;
}

char checkRequest(HttpRequest request)
{
    if(!request.protocol && !request.path && !request.method)
        return http_invalid;
   
    if(request.method == NULL)
        return http_wrong_format;
    if(request.path == NULL)
        return http_wrong_format;
    if(request.method == NULL)
        return http_wrong_format;

    if(isEmptyTable(request.header))
        return http_message_only;

    if(request.content == NULL)
        return http_no_content;

    if(getValueTable(request.header,"Content-Length")==NULL)
        return  http_invalid;

    return http_valid;
}

HttpObject newHttpObject(Tokener* tokener,int headerTableSize,int charLimit)
{
    HttpObject object = (HttpObject){NULL,newHashTable( headerTableSize),NULL};
    char* str = (char*)getNextToken(tokener,charLimit);
    
    if(str==NULL)
        return object;
    
    object.messageLine = (char*)malloc(strlen(str)+1);
    strcpy(object.messageLine,str);

    str = (char*)getNextToken(tokener,charLimit);
    while(str!=NULL && strlen(str)!=0)
    {   
        char* key = strtok(str,":");
        char* value = strtok(NULL,"");
        if(value == NULL)
            return object;
        addEntryTable(object.header,newEntry(key,value));
        str = (char*)getNextToken(tokener,charLimit);
    }

    
    char* sizePtr = getValueTable(object.header,"Content-Length");

    if(sizePtr==NULL || str==NULL)
        return object;
    
    str = (char*)getNextToken(tokener,charLimit);
    
    int size = atoi(sizePtr);
    object.content = (char*)malloc(size+1);
    strcpy(object.content,"");
    char first = 1;
    
    while(str!=NULL)
    {
        if(strlen(object.content)+strlen(str)+1>=size)
        {
            return object;
        }
        if(!first)
        {
            strcat(object.content,"\n");
        }
        else 
            first = 0;
        
        strcat(object.content,str);
        str = (char*)getNextToken(tokener,charLimit);
    } 

    return  object;
}

HttpRequest newHttpRequest(Tokener* tokener,int headerTableSize,int charLimit)
{
    HttpRequest object = (HttpRequest){NULL,NULL,NULL,newHashTable(headerTableSize),NULL};
    char* str = (char*)getNextToken(tokener,charLimit);
    if(str==NULL)
        return object;
    

    char* method = strtok(str," ");
    char* path = strtok(NULL," ");
    if(path == NULL)
        return object;
    char* protocol = strtok(NULL,"");

    if(protocol == NULL || !checkProtocol(protocol))
        return object;
    
    object.method = (char*)malloc(strlen(method)+1);
    object.path = (char*)malloc(strlen(path)+1);
    object.protocol = (char*)malloc(strlen(protocol)+1);

    strcpy(object.method,method);
    strcpy(object.path,path);
    strcpy(object.protocol,protocol);

    str = (char*)getNextToken(tokener,charLimit);
    while(str!=NULL && strlen(str)!=0)
    {     
        char* key = strtok(str,":");
        char*value = strtok(NULL,"");
        if(value == NULL)
            return object;
        addEntryTable(object.header,newEntry(key,value));
        str = (char*)getNextToken(tokener,charLimit);
    }

    
    char* sizePtr = getValueTable(object.header,"Content-Length");

    if(sizePtr==NULL || str==NULL)
        return object;
    
    str = (char*)getNextToken(tokener,charLimit);
    
    int size = atoi(sizePtr);
    object.content = (char*)malloc(size+1);
    strcpy(object.content,"");
    char first = 1;
    
    while(str!=NULL)
    {
        if(strlen(object.content)+strlen(str)+1>=size)
        {
            return object;
        }
        if(!first)
        {
            strcat(object.content,"\n");
        }
        else 
            first = 0;
        
        strcat(object.content,str);
        str = (char*)getNextToken(tokener,charLimit);
    } 

    return  object;
}

HttpResponse newHttpResponse(const char* content,HttpStatus status,int headerTableSize)
{
    HttpResponse response = (HttpResponse){status,newHashTable(headerTableSize),NULL,NULL};

    response.protocol = (char*)malloc(strlen("HTTP/1.1")+1);
    strcpy(response.protocol,"HTTP/1.1");
    if(content!=NULL)
    {
        response.content = (char*)malloc(strlen(content)+1);
        strcpy(response.content,content);
    }

    return  response;
}

void deleteHttpObject(HttpObject* object)
{
    if(object->content!=NULL)
    {
        free(object->content);
        object->content = NULL;
    }
    if(object->messageLine!=NULL)
    {
        free(object->messageLine);
        object->messageLine = NULL;
    }
    deleteTable(&object->header);
}

void deleteHttpRequest(HttpRequest* object)
{
    if(object->content!=NULL)
    {
        free(object->content);
        object->content = NULL;
    }
    if(object->method!=NULL)
    {
        free(object->method);
        object->method = NULL;
    }
    if(object->path!=NULL)
    {
        free(object->path);
        object->path = NULL;
    }
    if(object->protocol!=NULL)
    {
        free(object->protocol);
        object->protocol = NULL;
    }
    deleteTable(&object->header);
}

void deleteHttpResponse(HttpResponse* object)
{
    if(object->content!=NULL)
    {
        free(object->content);
        object->content = NULL;
    }
    if(object->protocol!=NULL)
    {
        free(object->protocol);
        object->protocol = NULL;
    }
    deleteTable(&object->header);
}

void setProtocol(HttpResponse* response,const char* protocol)
{
    if(response->protocol!=NULL)
        free(response->protocol);

    response->protocol = (char*)malloc(strlen(protocol)+1);
    strcpy(response->protocol,protocol);
}

char* HttpResponseToString(HttpResponse response)
{
    if(response.protocol==NULL)
        return NULL;
    
    char* str = (char*)malloc(strlen(response.protocol)+strlen(response.status.text)+6);
    strcpy(str,response.protocol);
    
    char status[4]; 
    sprintf(status,"%d",response.status.code);
    
    strcat(str," ");
    strcat(str,status);
    strcat(str," ");
    strcat(str,response.status.text);

    for(int i=0;i<response.header.size;i++)
    {
        EntryNode* list = response.header.data[i];
        while(list!=NULL)
        {
            str = strrealoc(str,strlen(str)+3
                +strlen(list->data.key)+strlen(list->data.value));
            
            strcat(str,"\n");
            strcat(str,list->data.key);
            strcat(str,":");
            strcat(str,list->data.value);

            list = list->next;
        }
    }

    if(response.content!=NULL)
    {
        str = strrealoc(str,strlen(str)+2+strlen(response.content));
        strcat(str,"\n\n");
        strcat(str,response.content);
    }

    return str;
}
char* HttpRequestToString(HttpRequest request)
{
    if(request.protocol==NULL)
        return NULL;
    if(request.method==NULL)
        return NULL;
    if(request.path==NULL)
        return NULL;
    
    char* str = (char*)malloc(strlen(request.protocol)+strlen(request.method)+strlen(request.path)+4);
    strcpy(str,request.method);
    strcat(str," ");
    strcat(str,request.path);
    strcat(str," ");
    strcat(str,request.protocol);


    for(int i=0;i<request.header.size;i++)
    {
        EntryNode* list = request.header.data[i];
        while(list!=NULL)
        {
            str = strrealoc(str,strlen(str)+3
                +strlen(list->data.key)+strlen(list->data.value));
            
            strcat(str,"\n");
            strcat(str,list->data.key);
            strcat(str,":");
            strcat(str,list->data.value);

            list = list->next;
        }
    }

    if(request.content!=NULL)
    {
        str = strrealoc(str,strlen(str)+2+strlen(request.content));
        strcat(str,"\n\n");
        strcat(str,request.content);
    }

    return str;
}
char* HttpObjectToString(HttpObject object)
{
    if(object.messageLine==NULL)
        return NULL;

    char* str = (char*)malloc(strlen(object.messageLine)+1);
    strcpy(str,object.messageLine);

    for(int i=0;i<object.header.size;i++)
    {
        EntryNode* list = object.header.data[i];
        while(list!=NULL)
        {
            str = strrealoc(str,strlen(str)+3
                +strlen(list->data.key)+strlen(list->data.value));
            
            strcat(str,"\n");
            strcat(str,list->data.key);
            strcat(str,":");
            strcat(str,list->data.value);

            list = list->next;
        }
    }

    if(object.content!=NULL)
    {
        str = strrealoc(str,strlen(str)+2+strlen(object.content));
        strcat(str,"\n\n");
        strcat(str,object.content);
    }

    return str;
}

JSONObject JSONParseRequest(HttpRequest request,int tableSize,int charLimit)
{
    Tokener tokener = createTokener(request.content,LF_TEXT_MODE);
    JSONObject object = getJSONObject(&tokener,tableSize,charLimit);
    deleteTokener(&tokener);
    return object;
}

HttpResponse HTMLResponse(const char* filepath,HttpStatus status,int headerTableSize,int charLimit)
{
    Tokener tokener = createTokener(filepath,LF_FILE_MODE);
    char* token = (char*)getNextToken(&tokener,charLimit); 
    char* str = (char*)malloc(1);
    char first = 1;
    strcpy(str,"");
    while(token!=NULL)
    {
        str = strrealoc(str,strlen(str)+strlen(token)+2);
        if(!first)
        {
            strcat(str,"\n");
        }
        else 
            first = 0;
        strcat(str,token);
        token = (char*)getNextToken(&tokener,charLimit); 
    }
    
    HttpResponse response= newHttpResponse(str,status, headerTableSize);
    
    char *server = server_info();
    char *date = getDate();
    char size[10];
    sprintf(size," %ld",strlen(response.content)+1);
    addEntryTable(response.header,newEntry("Date",date));
    addEntryTable(response.header,newEntry("Server",server));
    addEntryTable(response.header,newEntry("Connection"," keep-alive"));
    addEntryTable(response.header,newEntry("Content-Length",size));
    addEntryTable(response.header,newEntry("Content-Type"," text/html; charset=UTF-8"));
    
    free(server);
    free(date);
    free(str);
    deleteTokener(&tokener);
    
    return response;
}

HttpResponse JSONResponse(JSONObject obj,HttpStatus status,int headerTableSize)
{
    return newHttpResponse("",status,headerTableSize);
}