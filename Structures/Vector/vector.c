#include "vector.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void Vector_Expand(Vector* expand_vector)
{
    expand_vector->count++;
    expand_vector->data = realloc(
        expand_vector->data,
        expand_vector->count * expand_vector->data_size
    );
    if(expand_vector->data == NULL)
        expand_vector->count = 0;
    return;
}
static void Vector_Shrink(Vector* shrink_vector)
{
    if(Vector_IsEmpty(shrink_vector) || shrink_vector->count == 1)
    {
        Vector_Free(shrink_vector);
        return;
    }
    shrink_vector->count--;
    shrink_vector->data = realloc(
        shrink_vector->data,
        shrink_vector->count * shrink_vector->data_size
    );
    return;
}

static void* Data_Indexed(const Vector* vector, size_t index)
{
    return vector->data + (vector->data_size * index);
}

static void Data_Copy(void* destiny, const void* source, size_t count, size_t data_size)
{
    memcpy(destiny, source, count * data_size);
    return;
}

Vector Vector_Create(size_t element_size, size_t vector_size)
{
    Vector new_vector =
    {
        .data = calloc(vector_size, element_size),
        .count = vector_size,
        .data_size = element_size
    };
    if(new_vector.data == NULL)
        new_vector.count = new_vector.data_size = 0;
    return new_vector;
}
Vector Vector_CreateEmpty(size_t element_size)
{
    return (Vector)
    {
        .data = NULL,
        .count = 0,
        .data_size = element_size
    };
}
uint32_t Vector_IsEmpty(const Vector *vector)
{
    return (vector->count == 0 || vector->data == NULL);
}
void Vector_Free(Vector *free_vector)
{
    free_vector->count = 0;
    if(free_vector->data)
        free(free_vector->data);
    free_vector->data = NULL;
    return;
}
void Vector_Destroy(Vector* destroy_vector)
{
    Vector_Free(destroy_vector);
    destroy_vector->data_size = 0;
    return;
}

void Vector_Push(Vector *insert_vector, size_t index, const void *data)
{
    if(index >= insert_vector->count) return;
    else if(index == insert_vector->count - 1)
    {
        Vector_PushBack(insert_vector, data);
        return;
    }

    Vector_Expand(insert_vector);
    Data_Copy(
        Data_Indexed(insert_vector, index + 1),     // Copy to here
        Data_Indexed(insert_vector, index), // From here
        insert_vector->count - index - 1,           // This much
        insert_vector->data_size                // Bytes per chunk
    );
    Data_Copy(Data_Indexed(insert_vector, index), data, 1, insert_vector->data_size);
    return;
}
void Vector_PushFront(Vector *insert_vector, const void *data)
{
    Vector_Push(insert_vector, 0, data);
    return;
}
void Vector_PushBack(Vector *insert_vector, const void *data)
{
    Vector_Expand(insert_vector);
    Data_Copy(Data_Indexed(insert_vector, insert_vector->count - 1), data, 1, insert_vector->data_size);
    return;
}

const void *Vector_Peek(const Vector *peek_vector, size_t index)
{
    if(index >= peek_vector->count) return NULL;

    uint8_t* true_vector = (uint8_t*) peek_vector->data;
    return (void*) (true_vector + (index*peek_vector->data_size));
}
const void *Vector_PeekFront(const Vector *peek_vector)
{ return Vector_Peek(peek_vector, 0); }
const void *Vector_PeekBack(const Vector *peek_vector)
{ 
    size_t back_index = peek_vector->count - 1 < peek_vector->count ? peek_vector->count - 1 : 0;
    return Vector_Peek(peek_vector, back_index);
}

uint32_t Vector_CheckData(const Vector *search_vector, const void *search_key,
                          uint32_t (*comparator_function)(const void *, const void *))
{
    if(Vector_IsEmpty(search_vector))
        return 0;

    for(size_t i = 0; i < search_vector->count; i++)
    {
        if (comparator_function(search_key, Data_Indexed(search_vector, i)))
            return 1;
    }
    return 0;
}
uint32_t Vector_GetIndex(const Vector *search_vector, const void *search_key,
                         uint32_t (*comparator_function)(const void *, const void *))
{
    if(Vector_IsEmpty(search_vector))
        return UINT32_MAX;

    for(size_t i = 0; i < search_vector->count; i++)
    {
        if (comparator_function(search_key, Data_Indexed(search_vector, i)))
            return i;
    }
    return UINT32_MAX;
}

void Vector_Pop(Vector *pop_vector, size_t index)
{
    if(index >= pop_vector->count)
        return; // Covers Vector_IsEmpty() too
    else if(index == pop_vector->count - 1)
    {
        Vector_PopBack(pop_vector);
        return;
    }

    Data_Copy(
        Data_Indexed(pop_vector, index),     // Copy to here
        Data_Indexed(pop_vector, index + 1), // From here
        pop_vector->count - index,           // This much
        pop_vector->data_size                // Bytes per chunk
    );
    
    Vector_Shrink(pop_vector);
    if(Vector_IsEmpty(pop_vector))
        Vector_Free(pop_vector);
    return;
}
void Vector_PopFront(Vector *pop_vector)
{
    Vector_Pop(pop_vector, 0);
    return;
}
void Vector_PopBack(Vector *pop_vector)
{
    Vector_Shrink(pop_vector);
    return;
}

void Vector_Join(Vector *destiny_vector, const Vector *source_vector, uint32_t index)
{
    if(Vector_IsEmpty(source_vector))
        return;
    else if(Vector_IsEmpty(destiny_vector))
    {
        (*destiny_vector) = (*source_vector);
        return;
    }
    if(source_vector->data_size != destiny_vector->data_size)
        return;

    Vector new_vector = Vector_Create(
        destiny_vector->data_size,
        destiny_vector->count + source_vector->count
    );
    if(Vector_IsEmpty(&new_vector)) return;

    Data_Copy(new_vector.data, destiny_vector->data, index, destiny_vector->data_size);
    Data_Copy(
        Data_Indexed(&new_vector, index),
        source_vector->data,
        source_vector->count,
        source_vector->data_size
    );
    Data_Copy(
        Data_Indexed(&new_vector, index + source_vector->count),
        Data_Indexed(destiny_vector, index),
        destiny_vector->count - index,
        destiny_vector->data_size
    );

    Vector_Free(destiny_vector);
    (*destiny_vector) = new_vector;
    return;
}
void Vector_Append(Vector *destiny_vector, const Vector *source_vector)
{
    if(Vector_IsEmpty(source_vector))
        return;
    else if(Vector_IsEmpty(destiny_vector))
    {
        (*destiny_vector) = (*source_vector);
        return;
    }
    if(source_vector->data_size != destiny_vector->data_size)
        return;

    Vector new_vector = Vector_Create(
        destiny_vector->data_size,
        destiny_vector->count + source_vector->count
    );
    if(Vector_IsEmpty(&new_vector)) return;

    Data_Copy(new_vector.data, destiny_vector, destiny_vector->count, destiny_vector->data_size);
    Data_Copy(Data_Indexed(&new_vector, destiny_vector->count),
        source_vector->data,
        source_vector->count,
        source_vector->data_size
    );

    Vector_Free(destiny_vector);
    (*destiny_vector) = new_vector;
    return;
}
void Vector_Preappend(Vector *destiny_vector, const Vector *source_vector)
{
    if(Vector_IsEmpty(source_vector))
        return;
    else if(Vector_IsEmpty(destiny_vector))
    {
        (*destiny_vector) = (*source_vector);
        return;
    }
    if(source_vector->data_size != destiny_vector->data_size)
        return;

    Vector new_vector = Vector_Create(
        destiny_vector->data_size,
        destiny_vector->count + source_vector->count
    );
    if(Vector_IsEmpty(&new_vector)) return;

    Data_Copy(new_vector.data, source_vector, source_vector->count, source_vector->data_size);
    Data_Copy(Data_Indexed(&new_vector, source_vector->count),
        destiny_vector->data,
        destiny_vector->count,
        destiny_vector->data_size
    );

    Vector_Free(destiny_vector);
    (*destiny_vector) = new_vector;
    return;
}
