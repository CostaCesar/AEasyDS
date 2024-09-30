#include "doukedlist.h"
#include <stdint.h>
#include <stdlib.h>

static DoukedNode* CreateDoukedNode()
{
    return (DoukedNode*) malloc(sizeof(DoukedNode));
}
static void FreeDoukedNode(DoukedNode** free_node)
{
    free(*free_node);
    *free_node = NULL;
}

DoukedList DoukedList_Create()
{
    DoukedList output = {
        .start = NULL,
        .end = NULL,
        .size = 0
    };
    return output;
}
uint32_t DoukedList_IsEmpty(const DoukedList* list)
{
    return (list->size == 0 || list->end == NULL || list->start == NULL);
}
void DoukedList_Free(DoukedList* free_list)
{
    if(free_list->size == 0) return;
 
    DoukedNode* front_node = free_list->start, *back_node = free_list->end;
    for(uint32_t i = 0; i < free_list->size / 2; i++)
    {
        if(front_node != NULL)
        {
            front_node = free_list->start;
            FreeDoukedNode(&free_list->start);
            free_list->start = front_node;
        }

        if(back_node != NULL)
        {
            back_node = free_list->end->previous;
            FreeDoukedNode(&free_list->end);
            free_list->end = back_node;
        }
    }
    if(free_list->end == free_list->start && free_list->start == NULL)
        FreeDoukedNode(&free_list->start);

    *free_list = (DoukedList){0};

    return;
}

void DoukedList_Push(DoukedList* insert_list, uint32_t index, void* data)
{
    if(index > insert_list->size)
        return;
    if(index == 0)
    {
        DoukedList_PushFront(insert_list, data);
        return;
    }
    else if (index == insert_list->size)
    {
        DoukedList_PushBack(insert_list, data);
        return;
    }

    DoukedNode* new_node = CreateDoukedNode();
    if(new_node == NULL)
        return;

    DoukedNode *current_node;
    if(index > insert_list->size / 2) //  Go from back
    {
        current_node = insert_list->end;
        for(uint32_t i = insert_list->size; i > index; i--)
            current_node = current_node->previous;
        
    }
    else // Go from front (default path if index == size / 2)
    {
        current_node = insert_list->start;
        for(uint32_t i = 0; i < index; i++)
            current_node = current_node->next;
    }
    
    new_node->data = data;
    new_node->previous = current_node->previous;
    new_node->next = current_node;

    current_node->previous->next = new_node;
    current_node->previous = new_node;
    insert_list->size++;

    return;
}
void DoukedList_PushFront(DoukedList* insert_list, void* data)
{
    DoukedNode *new_node = CreateDoukedNode();
    if(!new_node) return;

    new_node->data = data;
    new_node->previous = NULL;
    new_node->next = insert_list->start ? insert_list->start : NULL;
    if(insert_list->start != NULL)
        insert_list->start->previous = new_node;
    insert_list->start = new_node;
    if(insert_list->size == 0)
        insert_list->end = new_node;

    insert_list->size++;
    return;
}
void DoukedList_PushBack(DoukedList* insert_list, void* data)
{
    DoukedNode *new_node = CreateDoukedNode();
    if(!new_node) return;
    
    new_node->data = data;
    new_node->next = NULL;
    new_node->previous = insert_list->end ? insert_list->end : NULL;
    if(insert_list->end != NULL)
        insert_list->end->next = new_node;
    insert_list->end = new_node;
    if(insert_list->size == 0)
        insert_list->start = new_node;
    
    insert_list->size++;
    return;
}

void* DoukedList_Peek(const DoukedList* get_list, uint32_t index)
{
    if(index > get_list->size)
        return NULL;

    if(index == 0)
        return DoukedList_PeekFront(get_list);
    else if (index == get_list->size - 1)
        return DoukedList_PeekBack(get_list);

    DoukedNode *current_node;    
    if(index > get_list->size / 2) //  Go from back
    {
        current_node = get_list->end;
        for(uint32_t i = get_list->size; i > index; i--)
            current_node = current_node->previous;
        
    }
    else // Go from front (default path if index == size / 2)
    {
        current_node = get_list->start;
        for(uint32_t i = 0; i < index; i++)
            current_node = current_node->next;
    }
    
    return current_node->data;
}
void* DoukedList_PeekFront(const DoukedList* get_list)
{
    if(DoukedList_IsEmpty(get_list))
        return NULL;

    return get_list->start->data;
}
void* DoukedList_PeekBack(const DoukedList* get_list)
{
    if(DoukedList_IsEmpty(get_list))
        return NULL;
    
    return get_list->end->data;
}

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