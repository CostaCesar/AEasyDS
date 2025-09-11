#ifndef AEASYDS_PRIORITY_QUEUE_HPP
#define AEASYDS_PRIORITY_QUEUE_HPP

#include <stdexcept>
#include <exception>
namespace Aeasyds
{
/// @brief      A queue that retrieves elements based on it's priority
/// @tparam     T Object type to be stored in the priority queue
/// @details    The queue is implemented as a min-heap array, so while access is constant time,
///             insertion and removal occur in logarithmic time. The queue has a maximum
///             capacity, after which no more elements can be added to the queue. The instance
///             must be provided with a priority function and a swap function to work
template <class T>
class PQueue
{
private:
    T* data;
    int capacity;
    int size;

    /// @brief Attributes priority to an element based on user-defined code
    /// @param element The element to be evaluated
    /// @return An priority index
    int (*GetPriority)(const T&);
    
    /// @brief Swaps two elements in-place based on user-defined code
    /// @param a The first element
    /// @param a The second element
    /// @details This allows the user to choose the best method for swapping
    void (*Swap)(T&, T&);

    /// @brief Gets the parent of the current element in the min-heap structure
    /// @param pos The index of the current element
    /// @return The index of the parent element
    int GetParent(int pos);

    /// @brief Gets the left child from the current element in the min-heap structure
    /// @param pos The index of the current element
    /// @return The index of the left child element
    int GetLeftChild(int pos);

    /// @brief Gets the right child from the current element in the min-heap structure
    /// @param pos The index of the current element
    /// @return The index of the right child element
    int GetRightChild(int pos);

public:
    /// @brief Solo constructor for the PQueue class
    /// @param _capacity The maximum capacity of the queue
    /// @param _priority_function A function that attributes a priority to the element
    /// @param _swap_function  A function that swaps the location of two elements
    PQueue(const int _capacity, int (*_priority_function)(const T&),
           void (*_swap_function)(T& , T&));
    ~PQueue();

    /// @brief Adds element to the list. The element is inserted by copy
    /// @param inserted Element to be inserted
    /// @return TRUE if element is inserted, FALSE otherwise
    bool Insert(const T& inserted);

    /// @brief Access the element in the front of the queue
    /// @return Reference to the element
    /// @warning This function returns garbage if the queue is empty
    T& Get();
    
    /// @brief Removes the element from the queue and returns it
    /// @return Copy of the element removed
    /// @warning This function returns garbage if the queue is empty
    T Remove();
    
    /// @brief Get the quantity of elements in the queue
    /// @returns Current size
    int Size() const;
    
    /// @brief Get the capacity of the queue
    /// @returns Queue's capacity
    int Capacity() const;
    
    /// @brief Determines if the queue has elements
    /// @returns TRUE if it's empty, FALSE otherwise
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