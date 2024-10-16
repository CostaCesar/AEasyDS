#ifndef VECTOR_VIEWER_H
#define VECTOR_VIEWER_H

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

Vector* VectorViewer_ChoseVector(Vector *vectors, uint32_t vectors_count)
{
    int selected = 0;
    for (int i = 0; i < vectors_count; i++)
    {
        printf("[*%d*] ", i);
    }
    printf("\n");
    printf("Vector Index >_ ");
    scanf("%d", &selected);

    if(selected >= vectors_count)
        return NULL;
    else return &vectors[selected];
}
void VectorViewer_ShowElement(Vector* vector, const void* data)
{
    if(data == NULL)
        return;

    printf("[");
    for (size_t i = 0; i < vector->data_size; i++)
    {
        printf("%02x ", *((uint8_t*) (data+i)) );
    }
    printf("]");

    return;
}
void VectorViewer_Show(Vector* vector)
{
    if(vector != NULL)
    {
        printf("[Vector at %p]\n", vector);
        printf("[Size: %llu (%llu bytes)]\n", 
            (long long unsigned) vector->count, 
            (long long unsigned)vector->count * vector->data_size);
        printf("[Element size: %llu bytes]\n", (long long unsigned) vector->data_size);
    }
    return;
}

void VectorViewer_Create(Vector **vectors, uint32_t *vectors_count)
{
    *vectors = (Vector*) realloc(*vectors, ((*vectors_count) + 1) * sizeof(Vector)); 
    if(*vectors == NULL)
    {
        *vectors_count = 0;
        return;
    }

    uint32_t element_size = 0;
    printf("Element size (bytes) >_ ");
    scanf("%u", &element_size);

    (*vectors)[(*vectors_count)] = Vector_CreateEmpty(element_size);
    (*vectors_count)++;
    return;
}

void VectorViewer_View(Vector *vectors, uint32_t vectors_count)
{
    if(vectors_count == 0)
        return;

    Vector* current_vector = VectorViewer_ChoseVector(vectors, vectors_count); 
    if(current_vector == NULL)
        return;
    
    int selected = 0;
    printf("[1] Entire Vector\n");
    printf("[2] Front Element\n");
    printf("[3] Last Element\n");
    printf("[4] Element At Index\n");
    printf("[0] Exit\n");

    printf(">_ ");
    scanf("%d", &selected);
    if(selected == 0) return;

    VectorViewer_Show(current_vector);
    const void* data;
    switch (selected)
    {
    case 1:
        for (size_t i = 0; i < current_vector->count; i++)
        {
            data = Vector_Peek(current_vector, i);
            VectorViewer_ShowElement(current_vector, data);
        }

        break;
    case 2:
        data = Vector_PeekFront(current_vector);
        VectorViewer_ShowElement(current_vector, data);
        break;
    case 3:
        data = Vector_PeekBack(current_vector);
        VectorViewer_ShowElement(current_vector, data);
        break;
    }
    printf("\n");
    return;
}

void VectorViewer_Insert(Vector* vectors, uint32_t vectors_count)
{
    if(vectors_count == 0)
        return;

    uint32_t selected = 0;
    uintptr_t value = 0;
    Vector* current_vector = NULL;
    
    printf("[1] At Position\n");
    printf("[2] In the Front\n");
    printf("[3] In the Back\n");
    printf("[0] Exit\n");

    printf(">_ ");
    scanf("%u", &selected);
    if(selected == 0) return;
    current_vector = VectorViewer_ChoseVector(vectors, vectors_count); 
    if(current_vector == NULL)
        return;

    printf("Value >_ ");
    scanf("%lu", &value);
    switch (selected)
    {
    case 1:
        VectorViewer_Show(current_vector);
        printf("Index >_ ");
        scanf("%u", &selected);
        if(selected >= current_vector->count)
            break;
        else Vector_Push(current_vector, selected, &value);
        break;
    case 2:
        VectorViewer_Show(current_vector);
        Vector_PushFront(current_vector, &value);
        break;
    case 3:
        VectorViewer_Show(current_vector);
        Vector_PushBack(current_vector, &value);
        break;
    }

    return;
}
void VectorViewer_Destroy(Vector** vectors, uint32_t* vector_count)
{
    Vector* current_vector = VectorViewer_ChoseVector(*vectors, *vector_count);
    if(current_vector == NULL)
        return;
    for(int i = 0; i < (*vector_count); i++)
    {
        if(&(*vectors)[i] == current_vector)
        {
            for(int j = i; j < (*vector_count)-1; j++)
                (*vectors)[j] = (*vectors)[j+1];

            Vector_Destroy(current_vector);
            (*vector_count)--;
            if(*vector_count == 0)
            {
                free(*vectors);
                *vectors = NULL;
            }
            else *vectors = realloc(*vectors, (*vector_count));
            
            return;
        }
    }

    return;
}

void VectorViewer_Main()
{
    Vector* vectors = NULL;
    uint32_t vectors_count = 0;
    
    int selected = 0;
    do
    {
        printf("<><><><><><><><><><><><>\n");
        for (int i = 0; i < vectors_count; i++)
        {
            printf("[*%d*] ", i);
        }
        printf("\n");
        
        printf("[1] Create new Vector\n");
        printf("[2] View Vector\n");
        printf("[3] Push into Vector\n");
        printf("[4] Remove from Vector\n");
        printf("[5] Join Vector\n");
        printf("[6] Pass Vector to Vector\n");
        printf("[7] Reverse Vector\n");
        printf("[8] Create Vector from Vector\n");
        printf("[9] Destroy Vector\n");
        printf("[0] Exit\n");

        printf(">_ ");
        scanf("%d", &selected);

        switch (selected)
        {
        case 1:
            VectorViewer_Create(&vectors, &vectors_count);
            break;
        case 2:
            VectorViewer_View(vectors, vectors_count);
            break;
        case 3:
            VectorViewer_Insert(vectors, vectors_count);
            break;
        case 9:
            VectorViewer_Destroy(&vectors, &vectors_count);
            break;
        case 0:
        default:
            break;
        }

    } while (selected != 0);
    printf("<><><><><><><><><><><><>\n");

    if(vectors != NULL)
        free(vectors);
    
    return;
}

#endif