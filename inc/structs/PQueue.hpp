#ifndef AEASYDS_PRIORITY_QUEUE_HPP
#define AEASYDS_PRIORITY_QUEUE_HPP

namespace Aeasyds
{
template <class T>
class PQueue
{
private:
    T* data;
    int capacity;
    int size;

    int GetIndex();

public:
    PQueue();
    ~PQueue();

    bool Insert(const T& inserted);
    T& GetLast();
    T& RemoveLast();

    int Size() const;
    int Capacity() const;
};

} // namespace Aeasyds


#endif