#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "Structures/LinkedList/linkedlist.h"

int main(int argc, char const *argv[])
{
    srand(101010);
    printf("I Was Here Before All...\n");

    LinkedList list = LinkedList_Create();
    
    for (size_t i = 0; i < 10; i++)
    {
        LinkedList_PushBack(&list, (void*) (size_t)(rand() % 100));
        printf("[%llu] ", (size_t) LinkedList_PeekBack(&list));
        if(i % 2) LinkedList_PopBack(&list);
    }
    printf("\n");

    for (size_t i = 10; i > 0; i--)
    {
        printf("[%llu] ", (size_t) LinkedList_Peek(&list, i-1));
    }
    printf("\n");

    return 0;
}
