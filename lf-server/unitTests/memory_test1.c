#include "../lf_http.h"

void test1()
{
    printf("Test1:\n");

    Tokener tokener = createTokener("jsonTests/test1.json",LF_FILE_MODE);
    JSONObject jsonObject = getJSONObject(&tokener,5,1024);

    deleteTokener(&tokener);
    deleteTable(&jsonObject);
}
void test2()
{
    printf("Test2:\n");

    Tokener tokener = createTokener("jsonTests/test2.json",LF_FILE_MODE);
    JSONArray arr = getJSONArray(&tokener,4,1024);
    deleteJSONArray(&arr);
    deleteTokener(&tokener);
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