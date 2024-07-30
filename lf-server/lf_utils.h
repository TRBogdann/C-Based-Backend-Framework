
#include <stdio.h>

#define LF_FILE_MODE 0
#define LF_TEXT_MODE 1
#define LF_NOT_VALID 2
#define LF_EXPIRED 3

//Deletes the empty spaces at the beginning of a string
void strtrim(char* str);

char* strrealoc(char*str,int newSize);

typedef struct TokenerContext
{
    char* buffer; //= NULL;
    char last; //= 0;
    char* savePoint;
}TokenerContext;

typedef struct textContent{
    int type;
    char* txtContent;
    TokenerContext context;
}textContent;

typedef struct fileContent{
    int type;
    FILE*  fileContent; 
    TokenerContext context;
}fileContent;

// structure used for parsing JSON/XML content.
// Can be disposed using deleteTokener() function
// you can create a new instance with createTokener();
typedef union Tokener
{   
    int type;
    textContent text;
    fileContent file;
}Tokener;



/*
type = RNG_FILE_MODE to parse a JSON/XML/TXT/HTML file
type = RNG_TEXT_MODE to parse a string
*/
Tokener createTokener(const char* content,int type);
void deleteTokener(Tokener* t);

//Returns the nextLine from a tokener.
//Removes the '\n' character at the end of the string
//charLimit = the max lenght of the token
const char* getNextToken(Tokener *tokener,int charLimit);

//checks if the tokener contains valid content
char testTokener(Tokener t);
void refreshTokener(Tokener* tokener);

const char* getOSName();
char* server_info();
char* getDate();