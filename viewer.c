#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "Structures/LinkedList/linkedlist.h"
#include "Structures/Vector/vector.h"
#include "Structures/DoukedList/doukedlist.h"

#include "Structures/Vector/vector_viewer.h"

uint32_t comp_int(const void* a, const void* b)
{
    return ((int*)a) == ((int*)b);
}

int main(int argc, char const *argv[])
{
    srand(101010);
    printf("I Was Here Before All...\n");

    LinkedList list = LinkedList_Create();
    
    for (size_t i = 0; i < 10; i++)
    {
        LinkedList_PushBack(&list, (void*) (size_t)(rand() % 100));
        printf("[%llu] ", (long long unsigned) LinkedList_PeekBack(&list));
        if(i % 2) LinkedList_PopBack(&list);
    }
    printf("\n");
    for (size_t i = 10; i > 0; i--)
    {
        printf("[%llu] ", (long long unsigned) LinkedList_Peek(&list, i-1));
    }
    printf("\n");
    LinkedList_Reverse(&list);
    for (size_t i = 0; i < list.size; i++)
    {
        printf("[%llu] ", (long long unsigned) LinkedList_Peek(&list, i));
    }
    printf("\n");
    for (size_t i = list.size; i > 0; i--)
    {
        printf("[%llu] ", (long long unsigned) LinkedList_Peek(&list, i-1));
    }
    printf("\n");

    VectorViewer_Main();


    DoukedList dk_list = DoukedList_Create();

    for (size_t i = 1; i < 4; i++)
    {
        switch (i % 3)
        {
        case 0:
            DoukedList_PushBack(&dk_list, (void*) (i * 10));
            break;
        case 1:
            DoukedList_PushFront(&dk_list, (void*) (i * 100));
            break;
        case 2:
            DoukedList_Push(&dk_list, dk_list.size/2, (void*) (i * 1000));
            break;
        }
    }
    for (size_t i = 0; i < dk_list.size; i++)
    {
        printf("[%llu] ", (long long unsigned) DoukedList_Peek(&dk_list, i));
    }
    printf("\n");
    for (size_t i = 0; i < 3000; i++)
    {
        if(DoukedList_CheckData(&dk_list, (void*)i, comp_int))
        {
            printf("[%llu found at %u] ", (long long unsigned)i,
                DoukedList_GetIndex(&dk_list, (void*) i, comp_int));
        }
    }
    printf("\n");
    
    while(!DoukedList_IsEmpty(&dk_list))
    {
        DoukedList_PopFront(&dk_list);
        DoukedList_PopBack(&dk_list);
    }

    DoukedList_Free(&dk_list);
    return 0;
}
