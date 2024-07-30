#include "lf_types.h"
#include "lf_utils.h"

//all HashTable functions work on JSONObject
typedef HashTable JSONObject;

//An array of JSONObjects
//Can be disposed with deleteJSONArray()
typedef struct JSONArray
{
    JSONObject* objects;
    int size;
}JSONArray;

//uses a tokener to create a json object
JSONObject getJSONObject(Tokener* tokener,int tableSize,int charLimit);

//uses a tokener to create an array of json objects
JSONArray getJSONArray(Tokener* tokener,int tableSize,int charLimit);

JSONArray newJSONArray(int size);
void resizeJSONArray(JSONArray *arr,int newSize);
void deleteJSONArray(JSONArray *arr);

//return a shallow copy of the object
JSONObject getJSONObjectArr(JSONArray arr,int index);

void printJSONArray(JSONArray arr);