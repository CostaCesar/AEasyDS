#ifndef AEASYDS_UNORDERED_SET_HPP
#define AEASYDS_UNORDERED_SET_HPP

#include <stdexcept>
#include <exception>

namespace Aeasyds
{

/* Declaration Here */    

template <class T>
class USet
{
private:
    inline static const int k_start_cap = 10;
    inline static const float k_threshold = 0.7;

    template <class U>
    struct UPos
    {
        bool is_virgin = true;
        bool is_vacant = true;
        U value;
    };
private:
    int m_size;
    int m_capacity;
    UPos<T>* m_data;

    int m_first_index;
    int m_last_index;

    unsigned int (*M_Hash)(const T&);
    bool (*M_Equal)(const T&, const T&);

    int M_GetNextIndex(int current_pos, int current_i);
    bool M_HasReachedThreshold();
    void M_Expand();

    void M_AdjustFirstIndex();
    void M_AdjustLastIndex();

public:
    USet(unsigned int (*hash_function_)(const T&), bool (*equal_function_)(const T&, const T&), int start_capacity_ = k_start_cap);
    ~USet();

    bool IsEmpty();
    bool IsFull();
    int GetSize();
    int GetCapacity();

    bool Contains(const T& element);
    void Add(const T& element);
    void Remove(const T& element);

    T& GetFirst();
    T& GetLast();
    // TODO: Iterators
};

/* Implementation Here */

template <class T>
USet<T>::USet(unsigned int (*hash_function_)(const T&), bool (*equal_function_)(const T&, const T&), int start_capacity_)
{
    if(start_capacity_ < 1)
        throw std::bad_array_new_length();

    this->m_capacity = start_capacity_;
    this->m_size = 0;
    
    this->m_data = new UPos<T>[m_capacity];
    if(this->m_data == nullptr)
        throw std::bad_alloc();

    this->M_Hash = hash_function_;
    this->M_Equal = equal_function_;

    this->m_first_index = m_capacity;
    this->m_last_index = -1;
}
template <class T>
USet<T>::~USet()
{
    delete[] this->m_data;
}
template <class T>
int USet<T>::M_GetNextIndex(int current_pos, int current_i)
{
    return (current_pos + (current_i * current_i)) % m_capacity;
}
template <class T>
bool USet<T>::IsEmpty()
{
    return this->m_size == 0;
}
template <class T>
bool USet<T>::IsFull()
{
    return this->m_capacity == m_size;
}
template <class T>
int USet<T>::GetSize()
{
    return m_size;
}
template <class T>
int USet<T>::GetCapacity()
{
    return m_capacity;
}
template <class T>
bool USet<T>::M_HasReachedThreshold()
{
    float cur = (static_cast<float>(this->m_size) / static_cast<float>(this->m_capacity));
    return cur > k_threshold;
}
template <class T>
void USet<T>::M_Expand()
{
    int old_capacity = m_capacity;
    m_capacity = (2) * old_capacity;
    if(m_capacity < old_capacity)
        throw std::logic_error("Could not expand capacity");

    UPos<T>* old_data = m_data;
    m_data = new UPos<T>[m_capacity];
    if(m_data == nullptr)
        throw std::bad_alloc();

    int old_size = m_size;
    m_size = 0;
    for (int i = 0; i < old_capacity; i++)
    {
        if(old_data[i].is_vacant)
            continue;
        else Add(old_data[i].value);
    }

    delete[] old_data;
    m_size = old_size;
}
    
template <class T>
void USet<T>::M_AdjustFirstIndex()
{
    if(IsEmpty())
    {
        m_first_index = m_capacity;
        return;
    }
    for(int i = m_first_index + 1; i < m_last_index; i++)
    {
        if(m_data[i].is_vacant == false)
        {
            m_first_index = i;
            return;
        }
    }
}
template <class T>
void USet<T>::M_AdjustLastIndex()
{
    if(IsEmpty())
    {
        m_last_index = -1;
        return;
    }
    for(int i = m_last_index - 1; i >= m_first_index; i--)
    {
        if(m_data[i].is_vacant == false)
        {
            m_last_index = i;
            return;
        }
    }
}
template <class T>
bool USet<T>::Contains(const T& element)
{
    if(IsEmpty()) return false;

    int pos = M_Hash(element) % m_capacity;
    for (int i = 1; !m_data[pos].is_virgin; i++)
    {
        if((!m_data[pos].is_vacant) && M_Equal(m_data[pos].value, element))
            return true;
        else  pos = M_GetNextIndex(pos, i);
    }

    return false;
}
template <class T>
void USet<T>::Add(const T& element)
{
    if(IsFull())
        throw std::out_of_range("Set is full, and could not be expanded");

    if(M_HasReachedThreshold())
        M_Expand();

    int pos = M_Hash(element) % m_capacity;
    for (int i = 1; !m_data[pos].is_vacant; i++)
    {
        if(M_Equal(m_data[pos].value, element)) // Element already in Set
            return;
        pos = M_GetNextIndex(pos, i);
    }

    if(pos < m_first_index)
        m_first_index = pos;
    if(pos > m_last_index)
        m_last_index = pos;

    m_data[pos].value = element;
    m_data[pos].is_virgin = false;
    m_data[pos].is_vacant = false;
    m_size++;
}
template <class T>
void USet<T>::Remove(const T& element)
{
    if(IsEmpty())
        return;

    int pos = M_Hash(element) % m_capacity;
    for (int i = 1; !m_data[pos].is_virgin; i++)
    {
        if(M_Equal(m_data[pos].value, element)) // Element already in Set
        {
            m_data[pos].is_vacant = true;
            m_size--;
            if(pos == m_first_index)
                M_AdjustFirstIndex();
            if(pos == m_last_index)
                M_AdjustLastIndex();
            return;
        }
        pos = M_GetNextIndex(pos, i);
    }

    return; // Should not reach here, but nothing bad happens anyway
}
template <class T>
T& USet<T>::GetFirst()
{
    if(IsEmpty())
        throw std::runtime_error("Empty set was accessed by GetFirst()");

    return m_data[m_first_index].value;
}
template <class T>
T& USet<T>::GetLast()
{
    if(IsEmpty())
        throw std::runtime_error("Empty set was accessed by GetLast()");

    return m_data[m_last_index].value;
}

} // namespace Aeasyds

#endif