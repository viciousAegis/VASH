#include "headers.h"

// Initalize and return a pointer to a linked list node
LL_Node *LL_Node_init(DataType* val)
{
    LL_Node *n = malloc(sizeof(LL_Node));
    n->data = (DataType*) malloc(sizeof(DataType));
    n->data = val;
    n->next = NULL;
    return n;
}

// initilase and return pointer to datatype
DataType* initDataType(int pid, char* name, int processNumber)
{
    DataType* data = (DataType*) malloc(sizeof(DataType));
    data->pid = pid;
    data->name = name;
    data->processNumber = processNumber;
    return data;
}

// Initalize and return a pointer to a linked list
LL *LL_init()
{
    LL *l = malloc(sizeof(LL));
    l->head = NULL;
    l->size = 0;
    return l;
}

// 1 if empty, 0 otherwise
int LL_empty(LL *l)
{
    return l->size == 0;
}

// Returns size of a linked list
int LL_Size(LL *l)
{
    return l->size;
}

// Add a value to linked list position p (0 index)
// This means that the node at position p will have value = val
// after this operation
// p can range from [0, size]
void LL_add(LL *l, int p, DataType* val)
{
    int i = 0;
    LL_Node *curr = l->head;
    LL_Node *prev = NULL;
    while (i < p)
    {
        prev = curr;
        curr = curr->next;
        ++i;
    }
    LL_Node *n = LL_Node_init(val);
    if (prev == NULL)
    {
        n->next = l->head;
        l->head = n;
    }
    else
    {
        prev->next = n;
        n->next = curr;
    }
    l->size += 1;
}

// Search for a value in a linked list
// Returns 1 if found, 0 if not found
int LL_search(LL *l, int pid)
{
    LL_Node *curr = l->head;
    while (curr != NULL)
    {
        if (curr->data->pid == pid)
        {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}
int LL_search_processNo(LL *l, int processNumber)
{
    LL_Node *curr = l->head;
    while (curr != NULL)
    {
        if (curr->data->processNumber == processNumber)
        {
            return curr->data->pid;
        }
        curr = curr->next;
    }
    return 0;
}

// Deletes the first value = val from the linked list
void LL_delete(LL *l, int pid)
{
    LL_Node *curr = l->head;
    LL_Node *prev = NULL;
    while (curr != NULL)
    {
        if (curr->data->pid == pid)
        {
            if (prev != NULL)
            {
                prev->next = curr->next;
            }
            else
            {
                if (curr->next == NULL)
                {
                    l->head = NULL;
                }
                else
                {
                    l->head = curr->next;
                }
            }
            free(curr);
            l->size -= 1;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void LL_print(LL *l)
{
    LL_Node *curr = l->head;
    while (curr != NULL)
    {
        printf("[%d] %s - %d", curr->data->processNumber, curr->data->name, curr->data->pid);
        curr = curr->next;
    }
    printf("\n");
}

void printJobs(LL* l)
{
    DataType* jobsArray = (DataType*) calloc(l->size ,sizeof(DataType));

    LL_Node* curr = l->head;
    int i = 0;
    while(curr != NULL)
    {
        jobsArray[i++] = *curr->data;
        curr = curr->next;
    }

    qsort(jobsArray, l->size, sizeof(DataType), compareJobs);

    for(int i = 0; i < l->size; i++)
    {
        printf("[%d] %s - %d\n", jobsArray[i].processNumber, jobsArray[i].name, jobsArray[i].pid);
    }
}

int compareJobs(const void* a, const void* b)
{
    DataType* jobA = (DataType*) a;
    DataType* jobB = (DataType*) b;

    return strcmp(jobA->name, jobB->name);
}