#ifndef AEASYDS_PRIORITY_QUEUE_HPP
#define AEASYDS_PRIORITY_QUEUE_HPP

#include <stdexcept>
#include <exception>
namespace Aeasyds
{
/* Declaration Here */  

/// @brief      A queue that retrieves elements based on it's priority
/// @tparam     T Object type to be stored in the priority queue
/// @details    The queue is implemented as a min-heap array, so while access is constant time,
///             insertion and removal occour in logarithmic time. The queue has a maximum
///             m_capacity, after which no more elements can be added to the queue. The instance
///             must be provided with a priority function and a swap function to work
template <class T>
class PQueue
{
private:
    T* m_data;
    int m_capacity;
    int m_size;

    /// @brief Attributes priority to an element based on user-defined code
    /// @param element The element to be evaluated
    /// @return An priority index
    int (*M_GetPriority)(const T&);
    
    /// @brief Swaps two elements in-place based on user-defined code
    /// @param a The first element
    /// @param a The second element
    /// @details This allows the user to choose the best method for swapping
    void (*M_Swap)(T&, T&);

    /// @brief Gets the parent of the current element in the min-heap structure
    /// @param pos The index of the current element
    /// @return The index of the parent element
    int M_GetParent(int pos);

    /// @brief Gets the left child from the current element in the min-heap structure
    /// @param pos The index of the current element
    /// @return The index of the left child element
    int M_GetLeftChild(int pos);

    /// @brief Gets the right child from the current element in the min-heap structure
    /// @param pos The index of the current element
    /// @return The index of the right child element
    int M_GetRightChild(int pos);

public:
    /// @brief Solo constructor for the PQueue class
    /// @param capacity_ The maximum m_capacity of the queue
    /// @param priority_function_ A function that attributes a priority to the element
    /// @param swap_function_  A function that swaps the location of two elements
    PQueue(const int capacity_, int (*priority_function_)(const T&),
           void (*swap_function_)(T& , T&));
    ~PQueue();

    /// @brief Adds element to the list. The element is inserted by copy
    /// @param inserted Element to be inserted
    /// @return TRUE if element is inserted, FALSE otherwise
    bool Insert(const T& inserted);

    /// @brief Access the element in the front of the queue
    /// @return Reference to the element
    /// @warning This function returns garbage if the queue is empty
    T& Get() const;
    
    /// @brief Removes the element from the queue and returns it
    /// @return Copy of the element removed
    /// @warning This function returns garbage if the queue is empty
    T Remove();
    
    /// @brief Get the quantity of elements in the queue
    /// @returns Current m_size
    int GetSize() const;
    
    /// @brief Get the m_capacity of the queue
    /// @returns Queue's m_capacity
    int GetCapacity() const;
    
    /// @brief Determines if the queue has elements
    /// @returns TRUE if it's empty, FALSE otherwise
    bool IsEmpty() const;

    /// @brief Clears queue from all elements
    /// @warning Queue capacity is retained
    void Clear();
};

/* Implementation Here */

template<class T>
int PQueue<T>::M_GetParent(int pos)
{
    return (pos - 1) / 2;
}
template<class T>
int PQueue<T>::M_GetLeftChild(int pos)
{
    return (2 * pos) + 1;
}
template<class T>
int PQueue<T>::M_GetRightChild(int pos)
{
    return (2 * pos) + 2;
}

template <class T>
inline PQueue<T>::PQueue(const int capacity_, int(*priority_function_)(const T&),
                         void (*swap_function_)(T&, T&))
{
    if(capacity_ < 1)
        throw std::bad_array_new_length();

    this->m_capacity = capacity_;
    this->m_size = 0;
    
    this->m_data = new T[capacity_];
    if(this->m_data == nullptr)
        throw std::bad_alloc();
    
    this->M_GetPriority = priority_function_;
    this->M_Swap = swap_function_;
}

template <class T>
inline PQueue<T>::~PQueue()
{
    if(this->m_capacity > 0 && this->m_data != nullptr)
        delete this->m_data;
    this->m_data = nullptr;
}

template <class T>
inline bool PQueue<T>::Insert(const T &inserted)
{
    if(m_size == m_capacity)
        return false;
    m_data[m_size] = inserted;

    int pos = m_size;
    for(int parent = M_GetParent(pos); ; parent = M_GetParent(pos))
    {
        if(M_GetPriority(m_data[parent]) <= M_GetPriority(m_data[pos]))
            break;

        M_Swap(m_data[parent], m_data[pos]);
        pos = parent;
    }
    m_size++;

    return true;
}

template <class T>
inline T &PQueue<T>::Get() const
{
    return m_data[0];
}

template <class T>
inline T PQueue<T>::Remove()
{
    T output = m_data[0];
    m_size--;

    m_data[0] = m_data[m_size];

    int pos = 0;
    int posRight = M_GetRightChild(pos);
    int posLeft = M_GetLeftChild(pos);
    while (
        (posRight < m_size && M_GetPriority(m_data[pos]) > M_GetPriority(m_data[posRight])) || 
        (posLeft < m_size && M_GetPriority(m_data[pos]) > M_GetPriority(m_data[posLeft]))
    )
    {
        if(M_GetPriority(m_data[posRight]) < M_GetPriority(m_data[posLeft]))
        {
            M_Swap(m_data[pos], m_data[posRight]);
            pos = posRight;
        }
        else
        {
            M_Swap(m_data[pos], m_data[posLeft]);
            pos = posLeft;
        }

        posRight = M_GetRightChild(pos);
        posLeft = M_GetLeftChild(pos);
    }
    return output;
}

template <class T>
inline int PQueue<T>::GetSize() const
{
    return this->m_size;
}
template <class T>
inline int PQueue<T>::GetCapacity() const
{
    return this->m_capacity;
}
template <class T>
inline bool PQueue<T>::IsEmpty() const
{
    return this->m_size < 1;
}

template <class T>
inline void PQueue<T>::Clear()
{
    m_size = 0;
}

} // namespace Aeasyds

#endif