#ifndef LINKED_LIST_H
#define LINKKED_LIST_H

#include <stdint.h>

typedef struct S_LinkNode
{
    void *data;
    struct S_LinkNode *next;
} LinkedNode;

typedef struct S_LinkList
{
    LinkedNode* start;
    uint32_t size;
} LinkedList;

LinkedList LinkedList_Create();
uint32_t LinkedList_IsEmpty(const LinkedList* list);
void LinkedList_Free(LinkedList* free_list);

void LinkedList_Push(LinkedList* insert_list, uint32_t index, void* data);
void LinkedList_PushFront(LinkedList* insert_list, void* data);
void LinkedList_PushBack(LinkedList* insert_list, void* data);

void* LinkedList_Peek(const LinkedList* get_list, uint32_t index);
void* LinkedList_PeekFront(const LinkedList* get_list);
void* LinkedList_PeekBack(const LinkedList* get_list);

uint32_t LinkedList_CheckData(const LinkedList* search_list, const void* search_key,
    uint32_t (*comparator_function)(const void*,const void*));
uint32_t LinkedList_GetIndex(const LinkedList* search_list, const void* search_key,
    uint32_t (*comparator_function)(const void*,const void*));
    
void LinkedList_Pop(LinkedList* pop_list, uint32_t index);
void LinkedList_PopFront(LinkedList* pop_list);
void LinkedList_PopBack(LinkedList* pop_list);

void LinkedList_Join(LinkedList* destiny_list, const LinkedList* source_list, uint32_t index);
void LinkedList_Append(LinkedList* destiny_list, const LinkedList* source_list);
void LinkedList_Preappend(LinkedList* destiny_list, const LinkedList* source_list);

void LinkedList_Reverse(LinkedList* reverse_list);

#endif