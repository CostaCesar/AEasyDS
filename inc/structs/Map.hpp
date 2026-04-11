#ifndef AEASYDS_MAP_HPP
#define AEASYDS_MAP_HPP

#include <stdexcept>
#include <exception>

namespace Aeasyds
{

/* Declaration Here */

/**
 * @brief A map class associating a key type with an value
 * @tparam K Object type used as the key
 * @tparam T Object type used as the value]
 * @details The map uses open addressing to store the values, and must receive a hash function for the K object
 */
template <class K, class T>
class Map
{
private:
    inline static const int k_start_cap = 10; // Default size for a set
    inline static const float k_threshold = 0.7; // Usage percentage after which the data array mus be expanded

    /**
     * @brief Internal class that stores the key-value pair as a single unit
     * @tparam U Object type of the key
     * @tparam V Object type of the value
     */
    template <class U, class V>
    struct MapPair
    {
        bool is_vacant; // Is the position free
        bool is_virgin; // Is the position not used before
        U key; // The key object
        V* value; // The value object

        /**
         * @brief Construct a new MapPair object
         */
        MapPair();
        /**
         * @brief Destroy the MapPair object
         */
        ~MapPair();
    };

private:
    int m_size; // Ammount of
    int m_filled;
    int m_capacity;
    MapPair<K, T>* m_data; // The stored objects

    /**
     * @brief The hash function for the key
     * @param K The given key object
     * @return The hash of the object
     */
    unsigned int (*M_Hash)(const K&);

    /**
     * @brief Expands the current capacity of the map
     * @throws "logic_error" if the new capacity is bad
     * @throws "bad_alloc" if map could not be expanded
     */
    void M_Expand();
    /**
     * @brief Obtains the next index for the position
     * 
     * @param current_pos The current position
     * @param current_i The current iteration
     * @return "int" The new position associated with this
     */
    int M_GetNextIndex(int current_pos, int current_i) const;
    /**
     * @brief Checks if the map has reached the threshold for expansion
     * 
     * @return "true" If expasion is needed for preserving hash efficiency,
     * @return "false" otherwise
     */
    bool M_HasReachedThreshold() const;
    
public:
    /**
     * @brief Construct a new Map object
     * 
     * @param hash_function_ The hash function for the keys
     * @param start_capacity_ (OPT) The starting capacirty of the map
     * @throws "bad_alloc" If map could not be initialised
     */
    Map(unsigned int (*hash_function_)(const K&), int start_capacity_ = k_start_cap);
    /**
     * @brief Destroy the Map object
     */
    ~Map();

    /**
     * @brief Get the ammount of objects that the map currently stores
     * 
     * @return "int" The ammount of elements
     */
    int GetSize() const;
    /**
     * @brief Get the ammount of objects that the map could store
     * 
     * @return "int" The capacity of the map
     */
    int GetCapacity() const;

    /**
     * @brief Gets if the map is empty
     * 
     * @return "true" If no elements are stored, 
     * @return "false" otherwise
     */
    bool IsEmpty() const;
    /**
     * @brief Checks if the map contains the key
     * 
     * @param key The key to be tested
     * @return "true" If an element associated with the key exists, 
     * @return "false" otherwise
     */
    bool Contains(const K& key) const;
    /**
     * @brief Wipes the map clean of all elements
     */
    void Clear();
    /**
     * @brief Get the element associated with the key
     * 
     * @param key The key used to retrieve
     * @return "T&" Reference to the element associated with the key
     * 
     * @throws "runtime_error" If no key-value pair is found
     */
    T& GetElement(const K& key);
    /**
     * @brief Adds a key associated with an element to the map
     * 
     * @param key The key to associate
     * @param value The element to be stored
     * 
     * @throws "bad_alloc" If map could not be expanded
     */
    void AddElement(const K& key, const T& value);
    /**
     * @brief Removes the element associated with the key
     * 
     * @param key The key to the association to be removed
     */
    void RemoveElement(const K& key);
};

/* Implementation Here */

template<class K, class T>
template<class U, class V>
Map<K,T>::MapPair<U, V>::MapPair() : key()
{
    this->is_vacant = true;
    this->is_virgin = true;
    this->value = nullptr;
}
template<class K, class T>
template<class U, class V>
Map<K,T>::MapPair<U, V>::~MapPair()
{
    if(this->value != nullptr)
        delete this->value;
}

template <class K, class T>
inline void Map<K, T>::M_Expand()
{
    int old_capacity = m_capacity;
    m_capacity = (2) * old_capacity;
    if(m_capacity < old_capacity)
        throw std::logic_error("New capacity < old capacity");

    MapPair<K, T>* old_data = m_data;
    m_data = new MapPair<K, T>[m_capacity];
    if(m_data == nullptr)
        throw std::bad_alloc();

    int old_size = m_size;
    m_size = 0;
    int old_filled = m_filled;
    m_filled = 0;

    for (int i = 0; i < old_capacity; i++)
    {
        if(old_data[i].is_vacant)
            continue;
        else AddElement(old_data[i].key, *old_data[i].value);
    }

    delete[] old_data;
    m_size = old_size;
    m_filled = old_filled;
}

template <class K, class T>
inline int Map<K, T>::M_GetNextIndex(int current_pos, int current_i) const
{
    return (current_pos + (current_i * current_i)) % m_capacity;
}

template <class K, class T>
inline bool Map<K, T>::M_HasReachedThreshold() const
{
    float cur = (static_cast<float>(this->m_filled) / static_cast<float>(this->m_capacity));
    return cur > k_threshold;
}

template <class K, class T>
inline Map<K, T>::Map(unsigned int (*hash_function_)(const K &), int start_capacity_)
{
    this->m_capacity = start_capacity_;
    this->m_size = 0;
    this->m_filled = 0;
    
    this->m_data = new MapPair<K, T>[m_capacity];
    if(this->m_data == nullptr)
        throw std::bad_alloc();

    this->M_Hash = hash_function_;
}


template <class K, class T>
Map<K, T>::~Map()
{
    if(this->m_data != nullptr)
        delete[] this->m_data;
}

template <class K, class T>
int Map<K, T>::GetSize() const
{
    return this->m_size;
}

template <class K, class T>
int Map<K, T>::GetCapacity() const
{
    return this->m_capacity;
}

template <class K, class T>
bool Map<K, T>::IsEmpty() const
{
    return this->m_size == 0;
}

template <class K, class T>
bool Map<K, T>::Contains(const K& key) const
{
    if(IsEmpty()) return false;

    auto pos = M_Hash(key) % m_capacity;
    for (int i = 1; !m_data[pos].is_virgin; i++)
    {
        if((!m_data[pos].is_vacant) && key == m_data[pos].key)
            return true;
        else pos = M_GetNextIndex(pos, i);
    }

    return false;
}

template <class K, class T>
void Map<K, T>::Clear()
{
    m_size = 0;
    m_filled = 0;

    for (int i = 0; i < m_capacity; i++)
    {
        m_data[i].is_virgin = true;
        m_data[i].is_vacant = true;
        
        delete m_data[i].value;
        m_data[i].value = nullptr;
    }
}

template <class K, class T>
T& Map<K, T>::GetElement(const K& key)
{
    if(!Contains(key))
        throw std::runtime_error("Tried to access key not present in map");
    // Now it's safe to search

    auto pos = M_Hash(key) % m_capacity;
    for (int i = 1; !m_data[pos].is_virgin; i++)
    {
        if(key == m_data[pos].key)
            return *m_data[pos].value;
        else pos = M_GetNextIndex(pos, i);
    }

    // Should not reach here
    throw std::runtime_error("Unable to access key not present in map");
}

template <class K, class T>
void Map<K, T>::AddElement(const K& key, const T& value)
{
    if(m_size == m_capacity)
        throw std::bad_alloc();

    if(M_HasReachedThreshold())
        M_Expand();

    auto pos = M_Hash(key) % m_capacity;
    for (int i = 1; !m_data[pos].is_vacant; i++)
    {
        // If element (key) is in the map, it will be overriden
        if(key == m_data[pos].key)
            break;
        else pos = M_GetNextIndex(pos, i);
    }

    m_data[pos].key = key;
    m_data[pos].value = new T(value);
    m_data[pos].is_virgin = false;
    m_data[pos].is_vacant = false;
    m_size++;
    m_filled++;
}

template <class K, class T>
void Map<K, T>::RemoveElement(const K& key)
{
    if(!Contains(key))
        return;

    auto pos = M_Hash(key) % m_capacity;
    for (int i = 1; !m_data[pos].is_virgin; i++)
    {
        if(key == m_data[pos].key)
        {
            delete m_data[pos].value;
            m_data[pos].value = nullptr;
            m_data[pos].is_vacant = true;

            m_size--;
            return;
        }
        else pos = M_GetNextIndex(pos, i);
    }
}

} // namespace Aeasyds

#endif // AEASYDS_MAP_HPP
