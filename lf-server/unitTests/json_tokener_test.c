#include "../lf_json.h"
#include <malloc.h>
#include <stdio.h>

void test1()
{
    printf("Test1:\n");

    Tokener tokener = createTokener("jsonTests/test1.json",LF_FILE_MODE);
    JSONObject jsonObject = getJSONObject(&tokener,5,1024);
    showAllEntries(jsonObject);

    deleteTokener(&tokener);
    deleteTable(&jsonObject);
}
void test2()
{
    printf("Test2:\n");

    Tokener tokener = createTokener("jsonTests/test2.json",LF_FILE_MODE);
    JSONArray arr = getJSONArray(&tokener,4,1024);
    printJSONArray(arr);

    deleteJSONArray(&arr);
    deleteTokener(&tokener);
}
void test3()
{
    printf("Test3:\n");
    
    Tokener tokener = createTokener("jsonTests/test3.json",LF_FILE_MODE);
    JSONObject jsonObject = getJSONObject(&tokener,5,1024);
    showAllEntries(jsonObject);
    
    deleteTokener(&tokener);
    deleteTable(&jsonObject);
}
void test4()
{
    printf("Test4:\n");

    Tokener tokener = createTokener("jsonTests/test4.json",LF_FILE_MODE);
    JSONObject jsonObject = getJSONObject(&tokener,5,1024);
    showAllEntries(jsonObject);

    deleteTokener(&tokener);
    deleteTable(&jsonObject);
}
int main()
{
    test1();
    test2();
    test4();
    return 0;
}