#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "Structures/LinkedList/linkedlist.h"
#include "Structures/Vector/vector.h"

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

    LinkedList_Free(&list);

    Vector vec1 = Vector_CreateEmpty(sizeof(int));
    Vector vec2 = Vector_CreateEmpty(sizeof(int));
    for (size_t i = 0; i < 20; i++)
    {
        if(i % 2) Vector_PushBack(&vec1, &i);
        else Vector_PushBack(&vec2, &i);
    }
    for(size_t i = vec1.count; i > 0; i--)
    {
        printf("[%d] ", *((int*) Vector_Peek(&vec1, i - 1)));
    }
    printf("\n");
    for(size_t i = vec2.count; i > 0; i--)
    {
        printf("[%d] ", *((int*) Vector_Peek(&vec2, i - 1)));
    }
    printf("\n");
    Vector_Join(&vec1, &vec2, vec1.count / 2);
    for(size_t i = 0; i < vec1.count; i++)
    {
        printf("[%d] ", *((int*) Vector_Peek(&vec1, i)));
    }
    printf("\n");
    Vector_Reverse(&vec1);
    for(size_t i = 0; i < vec1.count; i++)
    {
        printf("[%d] ", *((int*) Vector_Peek(&vec1, i)));
    }
    printf("\n");
    
    Vector_Destroy(&vec1);
    Vector_Destroy(&vec2);

    return 0;
}
