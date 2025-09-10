#ifndef AEASYDS_PRIORITY_QUEUE_HPP
#define AEASYDS_PRIORITY_QUEUE_HPP

#include <stdexcept>
#include <exception>
namespace Aeasyds
{
/// @brief Priority Queue
/// @tparam T Object type to be stored in the priority queue
template <class T>
class PQueue
{
private:
    T* data;
    int capacity;
    int size;

    int (*GetPriority)(const T&);
    void (*Swap)(T&, T&);

    int GetParent(int pos);
    int GetLeftChild(int pos);
    int GetRightChild(int pos);

public:
    PQueue(const int _capacity, int (*_priority_function)(const T&),
           void (*_swap_function)(T& , T&));
    ~PQueue();

    bool Insert(const T& inserted);
    T& Get();
    T Remove();

    int Size() const;
    int Capacity() const;
    bool IsEmpty() const;
};

/* Implementation Here */

template<class T>
int PQueue<T>::GetParent(int pos)
{
    return (pos - 1) / 2;
}
template<class T>
int PQueue<T>::GetLeftChild(int pos)
{
    return (2 * pos) + 1;
}
template<class T>
int PQueue<T>::GetRightChild(int pos)
{
    return (2 * pos) + 2;
}

template <class T>
inline PQueue<T>::PQueue(const int _capacity, int(*_priority_function)(const T&),
                         void (*_swap_function)(T&, T&))
{
    if(_capacity < 1)
        throw std::bad_array_new_length();

    this->capacity = _capacity;
    this->size = 0;
    
    this->data = new T[_capacity];
    if(this->data == nullptr)
        throw std::bad_alloc();
    
    this->GetPriority = _priority_function;
    this->Swap = _swap_function;
}

template <class T>
inline PQueue<T>::~PQueue()
{
    if(this->capacity > 0 && this->data != nullptr)
        delete this->data;
    this->data = nullptr;
}

template <class T>
inline bool PQueue<T>::Insert(const T &inserted)
{
    if(size == capacity)
        return false;
    data[size] = inserted;

    int pos = size;
    for(int parent = GetParent(pos); ; parent = GetParent(pos))
    {
        int a = GetPriority(data[parent]), b = GetPriority(data[pos]);
        if(a <= b) break;

        Swap(data[parent], data[pos]);
        pos = parent;
    }
    size++;

    return true;
}

template <class T>
inline T &PQueue<T>::Get()
{
    return data[0];
}

template <class T>
inline T PQueue<T>::Remove()
{
    T output = data[0];
    size--;

    data[0] = data[size];

    int pos = 0;
    int posRight = GetRightChild(pos);
    int posLeft = GetLeftChild(pos);
    while (
        (posRight < size && GetPriority(data[pos]) > GetPriority(data[posRight])) || 
        (posLeft < size && GetPriority(data[pos]) > GetPriority(data[posLeft]))
    )
    {
        if(GetPriority(data[posRight]) < GetPriority(data[posLeft]))
        {
            Swap(data[pos], data[posRight]);
            pos = posRight;
        }
        else
        {
            Swap(data[pos], data[posLeft]);
            pos = posLeft;
        }

        posRight = GetRightChild(pos);
        posLeft = GetLeftChild(pos);
    }
    return output;
}

template <class T>
inline int PQueue<T>::Size() const
{
    return this->size;
}
template <class T>
inline int PQueue<T>::Capacity() const
{
    return this->capacity;
}
template <class T>
inline bool PQueue<T>::IsEmpty() const
{
    return this->size < 1;
}



} // namespace Aeasyds

#endif