#include "lf_utils.h"
#include <malloc.h>
#include <string.h>
#include <sys/utsname.h>
#include <time.h>
#include <ctype.h>

void strtrim(char* str) {
    if (str == NULL) return; 

    int i = 0;
    int size = strlen(str);

    while (isspace(str[i]) && i<size) {
        i++;
    }

    if (i > 0 && i!=size) {
        memmove(str, str + i, strlen(str) - i + 1); // +1 to copy the null terminator
    }
}

char* strrealoc(char* str,int newSize)
{
    if(str == NULL)
        return NULL;
    if(newSize <= strlen(str)+1)
        return str;

    char* temp = (char*)realloc(str,newSize);
    
    if(temp == NULL)
    {
        temp = (char*)malloc(newSize);
        strcpy(temp,str);
        free(str);
    }

    return temp;
}

Tokener createTokener(const char *content, int type)
{
    Tokener t;
    t.type = type;
    if(content==NULL)
    {
        t.type = LF_NOT_VALID;
        return t;
    }

    if(type == LF_FILE_MODE)
    {
        t.file.fileContent = fopen(content,"rt");
        t.file.context.buffer = NULL;
        t.file.context.last = 0;
        t.file.context.savePoint = NULL;

        if(t.file.fileContent==NULL){
            fprintf(stderr,"File %s does not exis\n",content);
            t.type = LF_NOT_VALID;
        }
        return t;
    }
    
    if(type == LF_TEXT_MODE)
    {
        t.text.txtContent = (char*)malloc(strlen(content)+1);
        t.text.context.buffer = NULL;
        t.text.context.last = 0;
        t.text.context.savePoint = NULL;

        strcpy(t.text.txtContent,content);
        return t;
    }

    t.type = LF_NOT_VALID;
    return t;
}

void deleteTokener(Tokener* t)
{
    if(t->type==LF_FILE_MODE )
    {
        if(t->file.fileContent!=NULL){
            fclose(t->file.fileContent);
            t->file.fileContent = NULL;
        }

        if(t->file.context.buffer!=NULL)
        {
        free(t->file.context.buffer);
        t->file.context.buffer = NULL;
        }
        
        t->file.context.last = 0;
        t->file.context.savePoint = NULL;
        
    }

    if(t->type==LF_TEXT_MODE)
    {
        if(t->text.txtContent!=NULL){
            free(t->text.txtContent);
            t->text.txtContent = NULL;
        }
        
        if(t->text.context.buffer!=NULL)
        {
        free(t->text.context.buffer);
        t->text.context.buffer = NULL;
        }
        
        t->text.context.last = 0;
        t->text.context.savePoint = NULL;
    }

}

char testTokener(Tokener t)
{
    if(t.type==LF_FILE_MODE)
    {
        if(t.file.fileContent==NULL)
            return LF_NOT_VALID;
        if(feof(t.file.fileContent))
            return LF_EXPIRED;
    }

    if(t.type==LF_TEXT_MODE)
    {
        if(t.text.txtContent==NULL)
            return LF_NOT_VALID;
    }

    return t.type;
}

const char* getNextToken(Tokener *tokener,int charLimit)
{

    if(tokener == NULL)
        return NULL;

    if(tokener->type!=LF_TEXT_MODE && tokener->type!=LF_FILE_MODE)
        return NULL;

    //for charLimit<=0 the tokener is refreshed
    if(charLimit<=0)
    {
        if(tokener->file.context.buffer!=NULL)
            free(tokener->file.context.buffer);

        tokener->file.context.buffer=NULL;
        tokener->file.context.last = 0;
        tokener->file.context.savePoint = NULL;
        return NULL;
    }

    //deletes the previous buffer
    if(tokener->file.context.buffer!=NULL)
    {
        free(tokener->file.context.buffer);
        tokener->file.context.buffer = NULL;
    }

    //tokener can also be expired or invalid.
    int type = testTokener(*tokener);
    switch (type) {

        case LF_FILE_MODE:
            tokener->file.context.buffer = (char*)malloc(charLimit+1);
            fgets(tokener->file.context.buffer,charLimit,tokener->file.fileContent);
            if(tokener->file.context.buffer[strlen(tokener->file.context.buffer)-1]=='\n')
                tokener->file.context.buffer[strlen(tokener->file.context.buffer)-1] = 0;
            break;
        
        case LF_TEXT_MODE:
            tokener->file.context.buffer = (char*)malloc(charLimit+1);
            char* target = tokener->file.context.last==0?tokener->text.txtContent:NULL;
            char* token = strtok_r(target,"\n",&tokener->file.context.savePoint);
           
            if(token == NULL || strlen(token)>charLimit)
                {
                    free(tokener->file.context.buffer);
                    tokener->file.context.buffer = NULL;
                }
            else
                {
                    tokener->file.context.last = 1;
                    strcpy(tokener->file.context.buffer,token);
                }
        default:
            break;
    }   
    if(tokener->file.context.buffer == NULL)
       { 
        tokener->file.context.last = 0;
        tokener->file.context.savePoint = NULL;
       }
    return tokener->file.context.buffer;
}

const char* getOsName()
{
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Other";
    #endif
}  


char* server_info() {
    struct utsname system_info;

    if (uname(&system_info) == -1)
        return NULL;

    char* info = (char*)malloc(30);
     sprintf(info," %s (%s %s)",system_info.release,system_info.sysname,system_info.machine);
    return info;
}

char* getDate()
{
    char* date = (char*)malloc(100);
    time_t t = time(NULL);
    struct tm *dateTime = localtime(&t);
    strftime(date, 99, " %a, %d %b %Y %H:%M:%S GMT", dateTime);
    return date;
}

void refreshTokener(Tokener* tokener)
{
    if(tokener->type!=LF_FILE_MODE && tokener->type!=LF_TEXT_MODE)
        return; 
    
    if(tokener->file.context.buffer!=NULL)
    {
        free(tokener->file.context.buffer);
        tokener->file.context.buffer = NULL;
    }
    tokener->file.context.last = 0;
    tokener->file.context.savePoint = NULL;
}