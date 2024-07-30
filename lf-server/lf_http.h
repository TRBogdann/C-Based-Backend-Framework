#include "lf_json.h"

typedef struct HttpStatus
{
    int code;
    const char* text;
}HttpStatus;

//Informational responses
#define status_100 (HttpStatus){100,"Continue"}
#define status_101 (HttpStatus){101,"Switching Protocols"}
#define status_102 (HttpStatus){102,"Processing"}
#define status_103 (HttpStatus){103,"Early Hints"}

//Successful responses
#define status_200 (HttpStatus){200,"Ok"}
#define status_201 (HttpStatus){201,"Created"}
#define status_202 (HttpStatus){202,"Accepted"}
#define status_203 (HttpStatus){203,"Non-Authoritative Information"}
#define status_204 (HttpStatus){204,"No Content"}
#define status_205 (HttpStatus){205,"Reset Content"}
#define status_206 (HttpStatus){206,"Partial Content"}
#define status_207 (HttpStatus){207,"Multi-Status"}
#define status_208 (HttpStatus){208,"Already Reported"}
#define status_226 (HttpStatus){226,"IM Used"}

//Redirection messages
#define status_300 (HttpStatus){300,"Multiple Choices"}
#define status_301 (HttpStatus){301,"Moved Permanently"}
#define status_302 (HttpStatus){302,"Found"}
#define status_303 (HttpStatus){303,"See Other"}
#define status_304 (HttpStatus){304,"Not Modified"}
#define status_305 (HttpStatus){305,"Use Proxy"}
#define status_306 (HttpStatus){306,"unused"}
#define status_307 (HttpStatus){307,"Temporary Redirect"}
#define status_308 (HttpStatus){308,"Permanent Redirect"}

// Client Error responses
#define status_400 (HttpStatus){400, "Bad Request"}
#define status_401 (HttpStatus){401, "Unauthorized"}
#define status_402 (HttpStatus){402, "Payment Required"}
#define status_403 (HttpStatus){403, "Forbidden"}
#define status_404 (HttpStatus){404, "Not Found"}
#define status_405 (HttpStatus){405, "Method Not Allowed"}
#define status_406 (HttpStatus){406, "Not Acceptable"}
#define status_407 (HttpStatus){407, "Proxy Authentication Required"}
#define status_408 (HttpStatus){408, "Request Timeout"}
#define status_409 (HttpStatus){409, "Conflict"}
#define status_410 (HttpStatus){410, "Gone"}
#define status_411 (HttpStatus){411, "Length Required"}
#define status_412 (HttpStatus){412, "Precondition Failed"}
#define status_413 (HttpStatus){413, "Payload Too Large"}
#define status_414 (HttpStatus){414, "URI Too Long"}
#define status_415 (HttpStatus){415, "Unsupported Media Type"}
#define status_416 (HttpStatus){416, "Range Not Satisfiable"}
#define status_417 (HttpStatus){417, "Expectation Failed"}
#define status_418 (HttpStatus){418, "I'm a Teapot"}
#define status_421 (HttpStatus){421, "Misdirected Request"}
#define status_422 (HttpStatus){422, "Unprocessable Entity"}
#define status_423 (HttpStatus){423, "Locked"}
#define status_424 (HttpStatus){424, "Failed Dependency"}
#define status_425 (HttpStatus){425, "Too Early"}
#define status_426 (HttpStatus){426, "Upgrade Required"}
#define status_428 (HttpStatus){428, "Precondition Required"}
#define status_429 (HttpStatus){429, "Too Many Requests"}
#define status_431 (HttpStatus){431, "Request Header Fields Too Large"}
#define status_451 (HttpStatus){451, "Unavailable For Legal Reasons"}

// Server Error responses
#define status_500 (HttpStatus){500, "Internal Server Error"}
#define status_501 (HttpStatus){501, "Not Implemented"}
#define status_502 (HttpStatus){502, "Bad Gateway"}
#define status_503 (HttpStatus){503, "Service Unavailable"}
#define status_504 (HttpStatus){504, "Gateway Timeout"}
#define status_505 (HttpStatus){505, "HTTP Version Not Supported"}
#define status_506 (HttpStatus){506, "Variant Also Negotiates"}
#define status_507 (HttpStatus){507, "Insufficient Storage"}
#define status_508 (HttpStatus){508, "Loop Detected"}
#define status_510 (HttpStatus){510, "Not Extended"}
#define status_511 (HttpStatus){511, "Network Authentication Required"}

#define http_valid 0
#define http_invalid 1
#define http_message_only 2
#define http_no_content 3
#define http_wrong_format 4

typedef  struct HttpObject
{
    char* messageLine;
    HashTable header;
    char* content;  
}HttpObject;

typedef struct HttpRequest
{
    char* method;
    char* path;
    char* protocol;
    HashTable header;
    char* content;  
}HttpRequest;

typedef struct HttpResponse
{
    HttpStatus status;
    HashTable header;
    char* content;
    char* protocol;
}HttpResponse;

void printStatus(HttpStatus status);

HttpObject newHttpObject(Tokener* tokener,int headerTableSize,int charLimit);
HttpRequest newHttpRequest(Tokener* tokener,int headerTableSize,int charLimit);
HttpResponse newHttpResponse(const char* content,HttpStatus status,int headerTableSize);

void deleteHttpObject(HttpObject* object);
void deleteHttpRequest(HttpRequest* object);
void deleteHttpResponse(HttpResponse* object);

void setProtocol(HttpResponse* response,const char* protocol);

char* HttpResponseToString(HttpResponse response);
char* HttpRequestToString(HttpRequest request);
char* HttpObjectToString(HttpObject object);

JSONObject JSONParseRequest(HttpRequest request,int tableSize,int charLimit);
HttpResponse HTMLResponse(const char* filepath,HttpStatus status,int headerTableSize,int charLimit);
HttpResponse JSONResponse(JSONObject obj,HttpStatus status,int headerTableSize);

char checkRequest(HttpRequest request);

