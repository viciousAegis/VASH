#ifndef __DLL_H
#define __DLL_H

typedef int DataType;

typedef struct LL_Node
{
    DataType val;
    struct LL_Node *next;
} LL_Node;

typedef struct LL
{
    int size;
    LL_Node *head;
} LL;

LL *LL_init();

int LL_empty(LL *l);

void LL_add(LL *l, int p, DataType val);

int LL_search(LL *l, DataType val);

void LL_delete(LL *l, DataType val);

void LL_print(LL *l);

int LL_Size(LL *l);

LL* backgroundPIDs;

#endif
