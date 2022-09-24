#ifndef __DLL_H
#define __DLL_H

typedef struct _DataType
{
    int pid;
    char *name;
    int processNumber;
} DataType;

typedef struct LL_Node
{
    DataType* data;
    struct LL_Node *next;
} LL_Node;

typedef struct LL
{
    int size;
    LL_Node *head;
} LL;

DataType* initDataType(int pid, char* name, int processNumber);

LL *LL_init();

int LL_empty(LL *l);

void LL_add(LL *l, int p, DataType* data);

int LL_search(LL *l, int pid);

int LL_search_processNo(LL *l, int processNumber);

void LL_delete(LL *l, int pid);

void LL_print(LL *l);

int LL_Size(LL *l);

void printJobs(LL *l);

int compareJobs(const void *a, const void *b);

LL* backgroundPIDs;

#endif
