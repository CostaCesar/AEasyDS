#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
   uint8_t* data;
   size_t count;
   size_t data_size;
} Vector;

Vector Vector_Create(size_t element_size, size_t vector_size);
Vector Vector_CreateEmpty(size_t element_size);
uint32_t Vector_IsEmpty(const Vector* Vector);
void Vector_Free(Vector* free_vector);
void Vector_Destroy(Vector* destroy_vector);

void Vector_Push(Vector* insert_vector, size_t index, const void* data);
void Vector_PushFront(Vector* insert_vector, const void* data);
void Vector_PushBack(Vector* insert_vector, const void* data);

const void* Vector_Peek(const Vector* peek_vector, size_t index);
const void* Vector_PeekFront(const Vector* peek_vector);
const void* Vector_PeekBack(const Vector* peek_vector);

uint32_t Vector_CheckData(const Vector* search_vector, const void* search_key,
    uint32_t (*comparator_function)(const void*,const void*));
uint32_t Vector_GetIndex(const Vector* search_vector, const void* search_key,
    uint32_t (*comparator_function)(const void*,const void*));

void Vector_Pop(Vector* pop_vector, size_t index);
void Vector_PopFront(Vector* pop_vector);
void Vector_PopBack(Vector* pop_vector);

void Vector_Join(Vector* destiny_vector, const Vector* source_vector, uint32_t index);
void Vector_Append(Vector* destiny_vector, const Vector* source_vector);
void Vector_Preappend(Vector* destiny_vector, const Vector* source_vector);

void Vector_Reverse(Vector* reverse_vector);
#endif