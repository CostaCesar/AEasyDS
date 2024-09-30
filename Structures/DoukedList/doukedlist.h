#ifndef DOUBLE_LINKED_H
#define DOUBLE_LINKED_H

#include <stdint.h>

typedef struct S_DoukedNode
{
    void* data;
    struct S_DoukedNode *previous;
    struct S_DoukedNode *next;

} DoukedNode;
typedef struct
{
    uint32_t size;
    DoukedNode* start;
    DoukedNode* end;
} DoukedList;

DoukedList DoukedList_Create();
uint32_t DoukedList_IsEmpty(const DoukedList* list);
void DoukedList_Free(DoukedList* free_list);

void DoukedList_Push(DoukedList* insert_list, uint32_t index, void* data);
void DoukedList_PushFront(DoukedList* insert_list, void* data);
void DoukedList_PushBack(DoukedList* insert_list, void* data);

void* DoukedList_Peek(const DoukedList* get_list, uint32_t index);
void* DoukedList_PeekFront(const DoukedList* get_list);
void* DoukedList_PeekBack(const DoukedList* get_list);

uint32_t DoukedList_CheckData(const DoukedList* search_list, const void* search_key,
    uint32_t (*comparator_function)(const void*,const void*));
uint32_t DoukedList_GetIndex(const DoukedList* search_list, const void* search_key,
    uint32_t (*comparator_function)(const void*,const void*));
    
void DoukedList_Pop(DoukedList* pop_list, uint32_t index);
void DoukedList_PopFront(DoukedList* pop_list);
void DoukedList_PopBack(DoukedList* pop_list);

void DoukedList_Join(DoukedList* destiny_list, const DoukedList* source_list, uint32_t index);
void DoukedList_Append(DoukedList* destiny_list, const DoukedList* source_list);
void DoukedList_Preappend(DoukedList* destiny_list, const DoukedList* source_list);

void DoukedList_Reverse(DoukedList* reverse_list);

#endif