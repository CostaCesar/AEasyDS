#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

typedef struct
{
   void* data;
   size_t count;
   size_t data_size;
} Vector;

Vector Vector_Create(size_t element_size, size_t vector_size);
Vector Vector_CreateEmpty(size_t element_size);
uint32_t Vector_IsEmpty(const Vector* Vector);
void Vector_Free(Vector* free_vector);

void Vector_Push(Vector* insert_vector, size_t index, void* data);
void Vector_PushFront(Vector* insert_vector, void* data);
void Vector_PushBack(Vector* insert_vector, void* data);

void* Vector_Peek(Vector* peek_vector, size_t index);
void* Vector_PeekFront(Vector* peek_vector);
void* Vector_PeekBack(Vector* peek_vector);

uint32_t Vector_CheckData(const Vector* search_vector, const void* search_key,
    uint32_t (*comparator_function)(const void*,const void*));
uint32_t Vector_GetIndex(const Vector* search_vector, const void* search_key,
    uint32_t (*comparator_function)(const void*,const void*));

void Vector_Pop(Vector* pop_vector, size_t index);
void Vector_PopFront(Vector* spop_vector);
void Vector_PopBack(Vector* spop_vector);

void Vector_Join(Vector* destiny_vector, const Vector* source_vector, uint32_t index);
void Vector_Append(Vector* destiny_vector, const Vector* source_vector);
void Vector_Preappend(Vector* destiny_vector, const Vector* source_vector);

#endif