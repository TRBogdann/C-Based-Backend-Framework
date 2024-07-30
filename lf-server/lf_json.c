
#include "lf_json.h"
#include <stdlib.h>
#include <string.h>

char* JSONStrFormat(char* str)
{
    //deletes the ',' at the and of the value
    if(str[strlen(str)-1]==',')
    {
        str[strlen(str)-1]=0;
    }
    //deletes the "" from the string  values
    if(str[0]=='"' && str[strlen(str)-1]=='"')
        {
            memmove(str, str + 1, strlen(str));
            str[strlen(str)-1]=0;
        }
    return str;
}

char* getFullString(char* str,const char* delim,Tokener* tokener,int charLimit)
{
    char l = delim[0];
    char r = delim[0]=='[' ? ']' : '}';
    int left = 1;
    int right = 0;
    
    char* result = malloc(strlen(delim)+2);
    strcpy(result,delim);
    
    str = (char*)getNextToken(tokener,charLimit);
    while(left!=right && str!=NULL)
    {
        strcat(result,"\n");
        strtrim(str);
        char* temp = (char*)realloc(result,strlen(result)+strlen(str)+2);

        if(temp == NULL)
        {
            char* temp = malloc(strlen(result)+strlen(str)+2);
            strcpy(temp,result);
            free(result);
            result = temp;
        }
        else 
            result = temp;

        strcat(result,str);

        if(str[0]==l)
            left++;
        if(str[0]==r)
            right++;

        str = (char*)getNextToken(tokener,charLimit);
    }

    return result;
}

HashTable getJSONObject(Tokener *tokener,int tableSize,int charLimit)
{
    HashTable t = newHashTable(tableSize);
    
    char* str = (char*)getNextToken(tokener,charLimit);
    char end = 0;
    char first = 1;
    char valid = 1;

    while(str!=NULL && !end)
    {
        char* key;
        char* value;
        char* unformatted;
        strtrim(str);
        if(first)
        {
            if(str[0]==']')
                valid = 0;
            first = 0;
        }

        if(str[0]=='\n')
            valid = 0;

        int action = 1*abs(str[0]=='{')+2*abs(str[0]=='}')+(1-valid)*5;
        
        switch(action)
        {
            case 0:
                
                key = JSONStrFormat(strtok(str,":"));
                unformatted = strtok(NULL,"");
                strtrim(unformatted);
                if(unformatted[0]=='{')
                {
                    char* keep = (char*)malloc(strlen(key)+1);
                    strcpy(keep,key);
                    unformatted = getFullString(str,"{",tokener,charLimit);
                    addEntryTable(t,newEntry(keep,unformatted));
                    free(unformatted);
                    free(keep);
                }

                else if(unformatted[0]=='[')
                {
                    char* keep = (char*)malloc(strlen(key)+1);
                    strcpy(keep,key);
                    unformatted = getFullString(str,"[",tokener,charLimit);
                    addEntryTable(t,newEntry(keep,unformatted));
                    free(unformatted);
                    free(keep);
                }

                else
                {
                value = JSONStrFormat(unformatted);
                addEntryTable(t,newEntry(key,value));
                }

                break;
            
            case 2:
                if(str[strlen(str)-1]==',')
                    str[strlen(str)-1]=0;
                end = 1;
                break;
        }
        str = (char*)getNextToken(tokener,charLimit);
    }
    return t;
}

JSONArray newJSONArray(int size)
{
    JSONArray arr;
    arr.size = size;
    arr.objects = (JSONObject*)malloc(sizeof(JSONObject)*size);
    return arr;
}

void resizeJSONArray(JSONArray *arr,int newSize)
{
    if(newSize<arr->size)
        return;
    JSONObject* temp = (JSONObject*)realloc(arr->objects,newSize*sizeof(JSONObject));
    if(temp==NULL)
    {
            temp = (JSONObject*)malloc(newSize*sizeof(JSONObject));
            for(int i=0;i<arr->size;i++)
            {
                temp[i] = arr->objects[i];
            }
            free(arr->objects);

    }

    arr->objects = temp;
    arr->size = newSize;
}

void deleteJSONArray(JSONArray *arr)
{
    if(arr->objects==NULL)
        return;

    for(int i=0;i<arr->size;i++)
    {
        deleteTable(&(arr->objects[i]));
    }

    free(arr->objects);
    arr->size = 0;
    arr->objects = NULL;
}

JSONArray getJSONArray(Tokener *tokener,int tableSize,int charLimit)
{
    JSONArray arr;
    arr.objects = NULL;
    arr.size = 0;
    char* str = (char*)getNextToken(tokener,charLimit);
    
    char end = 0;
    
    while(!end)
    {
        JSONObject object = getJSONObject(tokener,tableSize,charLimit);
        if(isEmptyTable(object)){
            deleteTable(&object);
            end = 1;
        }
        else
            {
                if(arr.objects==NULL)
                    arr = newJSONArray(1);
                else
                    resizeJSONArray(&arr,arr.size+1);
                arr.objects[arr.size-1] = object;
            }
        
    }
    
    return arr;
}

JSONObject getJSONObjectArr(JSONArray arr,int index)
{
    if(index>=arr.size)
        return (JSONObject){NULL,0};
    else
        return arr.objects[index];
}

void printJSONArray(JSONArray arr)
{
    for(int i=0;i<arr.size;i++)
    {
        showAllEntries(arr.objects[i]);
    }
}