#ifndef AEASYDS_UNORDERED_SET_HPP
#define AEASYDS_UNORDERED_SET_HPP

#include <stdexcept>

namespace Aeasyds
{

/* Declaration Here */    

/// @brief      A set that stores elements uniquely
/// @tparam     T Object type to be stored in the set
/// @details    The set is implemented as an array indexed bya a hash function. The set can
///             expand beyond its original size to fit more elements, but it cannot shrink.
///             The expansion occours after a certain threshold below the limit, to improve
///             the access time, which is linear. The set uses quadratic probing for dealing
///             with collisions

template <class T>
class USet
{
private:
    inline static const int k_start_cap = 10; // Default size for a set
    inline static const float k_threshold = 0.7; // Usage percentage after which the data array mus be expanded

    /// @brief Structure to help the storage of elements in the set
    /// @tparam U Object type that the set stores
    template <class U>
    struct UPos
    {
        bool is_virgin = true; // Is this position free from the start
        bool is_vacant = true; // Is this position free for use
        U value; // Actual value stored
    };
private:
    int m_size; // Current number of elements stored
    int m_capacity; // Current size of the array
    UPos<T>* m_data; // Data array

    int m_first_index; // Index of the first element stored
    int m_last_index; // Index of the last element stored

    /// @brief Gives the hash for the object
    /// @param element The element to be evaluated
    /// @return The hash used for indexing in the set
    unsigned int (*M_Hash)(const T&);

    /// @brief Compares if two elements are equal
    /// @param element_a The first element
    /// @param element_b The second element
    /// @return TRUE if elements are equal, FALSE otherwise
    bool (*M_Equal)(const T&, const T&);

    /// @brief Gets the next index from the current position
    /// @param current_pos The base position
    /// @param current_pos The current iteration
    /// @return The next index from the current position
    int M_GetNextIndex(int current_pos, int current_i) const;

    /// @brief Determines if the set has reached the threshold for expansion
    /// @returns TRUE if the threshold was reached, FALSE otherwise
    bool M_HasReachedThreshold();

    /// @brief Expands the set to store more elements. Previous elements are preserved
    /// @warning Any iterators (eg. GetFirst, GetLast) are rendered invalid
    /// @throws "logic_error" if the new capacity is bad
    /// @throws "bad_alloc" if set could not be expanded
    void M_Expand();

    /// @brief Obtains the new index of the first element (index-wise) stored in the set
    void M_AdjustFirstIndex();

    /// @brief Obtains the new index of the last element (index-wise) stored in the set
    void M_AdjustLastIndex();

public:
    /// @brief Solo constructor for the Uset class
    /// @param hash_function_ A function that gives a hash to an object
    /// @param equal_function_ A function that compares if two objects are equal
    /// @param start_capacity_ (OPT) Starting capacity of the set
    /// @throws "bad_array_lenght" If given capacity is smaller than 1
    /// @throws "bad_alloc" If set could not be initialised
    USet(unsigned int (*hash_function_)(const T&), bool (*equal_function_)(const T&, const T&), int start_capacity_ = k_start_cap);
    ~USet();

    /// @brief Gets if the set contains any elements
    /// @returns TRUE if no elements are present, FALSE otherwise
    bool IsEmpty() const;

    /// @brief Gets if the set cannot store any more elements
    /// @returns TRUE if the set is tottaly filled, FALSE otherwise
    bool IsFull() const;

    /// @brief Gets the number of elements currently in the set
    /// @returns The number of elements stored
    int GetSize() const;

    /// @brief Gets the current capacity of the set
    /// @returns The maximum amount of elements that the set can store
    int GetCapacity() const;

    /// @brief Checks if the element is present in the set
    /// @param element The object that will be checked
    /// @return TRUE if it's present, FALSE otherwise
    bool Contains(const T& element) const;

    /// @brief Inserts an element in the set, if it's not present already
    /// @param element The object to be inserted
    /// @throws "bad_alloc" If set could not be expanded
    void Add(const T& element);

    /// @brief Removes an element from the set, if it's present
    /// @param element The object to be removed
    void Remove(const T& element);

    /// @brief Wipes clean the set, as if no element were inserted
    void Clear();

    /// @brief Gets the first element (index-wise) stored in the set
    /// @return A reference to the first element
    /// @warning This reference can be invalid after removal operations
    /// @throws "runtime_error" If the set is empty
    T& GetFirst() const;

    /// @brief Gets the last element (index-wise) stored in the set
    /// @return A reference to the last element
    /// @warning This reference can be invalid after removal operations
    /// @throws "runtime_error" If the set is empty
    T& GetLast() const;

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
int USet<T>::M_GetNextIndex(int current_pos, int current_i) const
{
    return (current_pos + (current_i * current_i)) % m_capacity;
}
template <class T>
bool USet<T>::IsEmpty() const
{
    return this->m_size == 0;
}
template <class T>
bool USet<T>::IsFull() const
{
    return this->m_capacity == m_size;
}
template <class T>
int USet<T>::GetSize() const
{
    return m_size;
}
template <class T>
int USet<T>::GetCapacity() const
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
bool USet<T>::Contains(const T& element) const
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
        throw std::bad_alloc();

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
        if(M_Equal(m_data[pos].value, element)) // Element found
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
inline void USet<T>::Clear()
{
    for (int i = 0; i < m_capacity; i++)
    {
        m_data[i].is_vacant = true;
        m_data[i].is_virgin = true;
    }
    
    this->m_size = 0;
}

template <class T>
T& USet<T>::GetFirst() const
{
    if(IsEmpty())
        throw std::runtime_error("Empty set was accessed by GetFirst()");

    return m_data[m_first_index].value;
}
template <class T>
T& USet<T>::GetLast() const
{
    if(IsEmpty())
        throw std::runtime_error("Empty set was accessed by GetLast()");

    return m_data[m_last_index].value;
}

} // namespace Aeasyds

#endif