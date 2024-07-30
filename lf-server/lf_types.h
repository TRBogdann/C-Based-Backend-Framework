//key-pair values.
//Create an instance wirh newEntry() function
//Can be disposed  using deleteEntry() function
typedef struct Entry
{
    char* key;
    char* value;
}Entry;

Entry newEntry(const char* key,const char* value);
void deleteEntry(Entry* entry);
void printEntry(Entry entry);

//A list of entries.
//Can be disposed with deleteEntryList() function
typedef struct EntryNode
{
    Entry data;
    struct EntryNode* next;
}EntryNode;

void deleteEntryList(EntryNode** list);
void insertFirstEntryList(EntryNode** list,Entry entry);
void insertLastEntryList(EntryNode** list,Entry entry);

//returns a shallow copy of an Entry
Entry getElementByKey(EntryNode* list,const char* key);
//returns a shallow of a value
char* getValueByKey(EntryNode* list,const char* key);
void showEntries(EntryNode* list);

//table of Entries
//Can be disposed with deleteTable()
//Can be created with newHashTable()
//random note: Maybe i could just use a list for JSONObjects instead of a table.Oh Well
typedef struct HashTable
{
    EntryNode** data;
    int size;
}HashTable;

HashTable newHashTable(int size);
void deleteTable(HashTable* table);
void addEntryTable(HashTable table,Entry e);
Entry getEntryTable(HashTable table,const char* key);
char* getValueTable(HashTable table,const char* key);
void showAllEntries(HashTable table);
char isEmptyTable(HashTable table);