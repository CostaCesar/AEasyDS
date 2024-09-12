#include "linkedlist.h"
#include <stdlib.h>
#include <stdint.h>

static LinkedNode* CreateLinkedNode()
{
    return (LinkedNode*) calloc(1, sizeof(LinkedNode));
}
static void FreeLinkedNode(LinkedNode** free_node)
{
    free(*free_node);
    *free_node = NULL;
}
LinkedList LinkedList_Create()
{
    return (LinkedList) {NULL, 0};
}
uint32_t LinkedList_IsEmpty(const LinkedList* list)
{
    if(list->size == 0)
    {
        if(list->start) return 2; // 0 size, bad pointer
        else return 1; // Is empty
    }
    else
    {
        if(list->start) return 0; // Not Empty
        else return 3; // NULL pointer, bad size
    }
}

void LinkedList_Free(LinkedList* free_list)
{
    LinkedNode* buffer = free_list->start;
    while (buffer)
    {
        buffer = buffer->next;
        free(free_list->start);
        free_list->start = buffer;
    }
    
    free_list->size = 0;
    free_list->start = NULL;

    return;
}

void LinkedList_Push(LinkedList* insert_list, uint32_t index, void* data)
{
    if(index > insert_list->size) return;
    else if (index == 0)
    {
        LinkedList_PushFront(insert_list, data);
        return;
    }

    LinkedNode* parent = insert_list->start;
    for (int i = 1; i < index; i++)
    {
        parent = parent->next;
    }

    LinkedNode* new_node = CreateLinkedNode();
    if(!new_node) return;

    insert_list->size++;
    new_node->data = data;
    new_node->next = parent->next;
    parent->next = new_node;
    
    return; 
}
void LinkedList_PushFront(LinkedList* insert_list, void* data)
{
    LinkedNode* new_node = CreateLinkedNode();
    if(!new_node) return;
    
    new_node->data = data;
    new_node->next = insert_list->start;

    insert_list->start = new_node;
    insert_list->size++;
    return;
}
void LinkedList_PushBack(LinkedList* insert_list, void* data)
{
    LinkedList_Push(insert_list, insert_list->size, data);
}
void* LinkedList_Peek(const LinkedList* get_list, uint32_t index)
{
    if(index < 0 || index >= get_list->size)
        return NULL;

    LinkedNode *output = get_list->start;
    for(size_t i = 0; i < index; i++)
        output = output->next;
    return output->data;
}
void* LinkedList_PeekFront(const LinkedList* get_list)
{
    return LinkedList_Peek(get_list, 0);
}
void* LinkedList_PeekBack(const LinkedList* get_list)
{
    return LinkedList_Peek(get_list, get_list->size - 1);
}
uint32_t LinkedList_CheckData(const LinkedList* search_list, const void* search_key,
    uint32_t (*comparator_function)(const void*,const void*))
{
    for (LinkedNode* node = search_list->start; node != NULL; node = node->next)
    {
        if(comparator_function(search_key, node->data))
            return 1;
    }
    return 0;
}
uint32_t LinkedList_GetIndex(const LinkedList* search_list, const void* search_key,
    uint32_t (*comparator_function)(const void*,const void*))
{
    uint32_t index = 0;
    for(LinkedNode* node = search_list->start; node != NULL; node = node->next)
    {
        if(comparator_function(search_key, node))
            break;
        else index++;
    }
    return index;
}
void LinkedList_Pop(LinkedList* pop_list, uint32_t index)
{
    if(LinkedList_IsEmpty(pop_list)) return;
    else if(index >= pop_list->size) return;

    pop_list->size--;
    LinkedNode* parent_node;
    LinkedNode* remove_node = pop_list->start;

    if(index == 0)
        pop_list->start = pop_list->start->next;
    else
    {
        for (size_t i = 0; i < index; i++)
        {
            parent_node = remove_node;
            remove_node = remove_node->next;
        }
        parent_node->next = remove_node->next;
    }
    FreeLinkedNode(&remove_node);
    return;
}

void LinkedList_PopFront(LinkedList* pop_list)
{
    LinkedList_Pop(pop_list, 0);
}
void LinkedList_PopBack(LinkedList* pop_list)
{
    LinkedList_Pop(pop_list, pop_list->size - 1);
}

void LinkedList_Join(LinkedList* destiny_list, const LinkedList* source_list, uint32_t index)
{
    if(LinkedList_IsEmpty(source_list)) return;
    else if(index >= destiny_list->size) return;

    destiny_list->size += source_list->size;
    if(LinkedList_IsEmpty(destiny_list))
    {
        *destiny_list = *source_list;
        return;
    }
    
    LinkedNode* pause_node = destiny_list->start;
    for (size_t i = 0; i < index; i++)
        pause_node = pause_node->next;
    
    LinkedNode* resume_node = pause_node->next;
    pause_node->next = source_list->start;
    for(pause_node; pause_node->next != NULL; pause_node = pause_node->next);
    pause_node->next = resume_node;
       
    return;
}
void LinkedList_Append(LinkedList* destiny_list, const LinkedList* source_list)
{
    LinkedList_Join(destiny_list, source_list, destiny_list->size - 1);
}
void LinkedList_Preappend(LinkedList* destiny_list, const LinkedList* source_list)
{
    LinkedList_Join(destiny_list, source_list, 0);
}