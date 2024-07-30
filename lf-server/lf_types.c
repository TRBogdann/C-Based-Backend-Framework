#include "lf_types.h"
#include <malloc.h>
#include <string.h>

//I ll assume you know how basic structures work 
// so i won t add any comments in this source file
// I am also very lazy :P

Entry newEntry(const char* key,const char* value)
{
    Entry e;

    e.key = (char*)malloc(strlen(key)+1);
    strcpy(e.key,key);

    e.value = (char*)malloc(strlen(value)+1);
    strcpy(e.value,value);

    return e;

}

void printEntry(Entry e)
{
    printf("%s:%s\n",e.key,e.value);
}

void deleteEntry(Entry* entry)
{
    free(entry->key);
    free(entry->value);
    entry->value = NULL;
    entry->key = NULL;
}

void deleteEntryList(EntryNode** list)
{
    if((*list)==NULL)
        return;
    
    deleteEntryList(&(*list)->next);
    
    deleteEntry(&(*list)->data);
    free((*list));
    (*list) = NULL;
}

void insertFirstEntryList(EntryNode** list,Entry entry)
{
    EntryNode *newNode = (EntryNode*)malloc(sizeof(EntryNode));
    newNode->data = entry;
    newNode->next = (*list);
    (*list) = newNode;
}

void insertLastEntryList(EntryNode** list,Entry entry)
{
    EntryNode *newNode = (EntryNode*)malloc(sizeof(EntryNode));
    newNode->data = entry;
    newNode->next = NULL;
    if((*list)==NULL)
    {
        (*list) = newNode;
        return;
    }
    EntryNode* temp = (*list);
    while(temp->next!=NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

Entry getElementByKey(EntryNode* list,const char* key)
{
    while(list!=NULL)
    {
        if(strcmp(list->data.key,key)==0)
            return list->data;
        list = list->next;
    }
    return (Entry){NULL,NULL};
}

char* getValueByKey(EntryNode* list,const char* key)
{
    while(list!=NULL)
    {
        if(strcmp(list->data.key,key)==0)
            return list->data.value;
        list = list->next;
    }
    return NULL;
}

void showEntries(EntryNode* list)
{
    while(list!=NULL)
    {
        printEntry(list->data);
        list = list->next;
    }
}


HashTable newHashTable(int size)
{
    HashTable t;
    t.size = size;
    t.data = (EntryNode**)calloc(size,sizeof(EntryNode*));
    return t;
}

int hash(int size,const char* key)
{
    return (int)(key[0])%size;
}

void deleteTable(HashTable *table)
{
    if(table == NULL)
        return;
    
    
    for(int i=0;i<table->size;i++)
    {
        deleteEntryList(&(table->data[i]));
    }
    
    if(table->data!=NULL)
    {
        free(table->data);
        table->data = NULL;
    }
    table->size = 0;
    
}

void addEntryTable(HashTable table,Entry e)
{
    if(table.size == 0)
        return;
    int i = hash(table.size,e.key);
    insertLastEntryList(&table.data[i],e);
}

Entry getEntryTable(HashTable table,const char* key)
{
    if(table.size==0)
        return (Entry){NULL,NULL};
    int i = hash(table.size,key);
    return getElementByKey(table.data[i],key);
}

char* getValueTable(HashTable table,const char* key)
{
    if(table.size==0)
        return NULL;
    int i = hash(table.size,key);
    return getValueByKey(table.data[i],key);
}

void showAllEntries(HashTable table)
{
    for(int i=0;i<table.size;i++)
    {
        showEntries(table.data[i]);
    }
}

char isEmptyTable(HashTable table)
{
    for(int i=0;i<table.size;i++)
    {
        if(table.data[i]!=NULL)
            return 0;
    }
    return 1;
}