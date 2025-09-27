#include <iostream>

#include "structs/PQueue.hpp"
#include "structs/USet.hpp"
#include "Random.hpp"

int main(int argc, char const *argv[])
{
    const int k_size = 1000;
    Aeasyds::Random::LFSR_64 generator(9876543210);

    auto GetPriority = [](const int& num){
        return num;
    };
    auto Swap = [](int& a, int& b){
        int x = a;
        a = b;
        b = x;
    };

    Aeasyds::PQueue<int> queue(k_size, GetPriority, Swap);
    
    for (int i = 0; i < k_size / 2; i++)
    {
        int a = (generator.Rand() % 0xffffffff) % 10;
        queue.Insert(a);
        std::cout << "Inserted " << a << std::endl;
    }
    while (queue.Size() > 5)
    {
        int b = queue.Remove();
        std::cout << "Removed " << b << std::endl;
    }
    std::cout << "Current size: " << queue.Size() << std::endl;
    std::cout << "Next on queue: " << queue.Get() << std::endl;

    generator.Seed(987654321);
    for (int i = 0; i < k_size / 2; i++)
    {
        int a = (generator.Rand() % 0xffffffff) % 10;
        queue.Insert(a);
        std::cout << "Inserted " << a << std::endl;
    }
    while (queue.Size() > 5)
    {
        int b = queue.Remove();
        std::cout << "Removed " << b << std::endl;
    }
    std::cout << "Current size: " << queue.Size() << std::endl;
    std::cout << "Next on queue: " << queue.Get() << std::endl;
    
    auto Equals = [](const int& a, const int& b){
        return a == b;
    };
    auto Hash1 = [](const int& num){
        Aeasyds::Random::LCG_64 hasher(num);
        return static_cast<unsigned int>(hasher.Rand());
    };
    auto Hash2 = [](const int& num){
        unsigned int x = num;
        x = ((x >> 16) ^ x) * 0x119de1f3u;
        x = ((x >> 16) ^ x) * 0x119de1f3u;
        x = (x >> 16) ^ x;
        return x;
    };
    
    Aeasyds::USet<int> set(Hash2, Equals, k_size);
    generator.Reset();
    for (size_t i = 0; i < k_size; i++)
    {
        bool b = false;
        int a = std::abs(static_cast<int>(generator.Rand()) % 1000);
        if(set.Contains(a))
            b = true;
        set.Add(a);
        
        if(b == true)
            std::cout << a << " was already in  the set" << std::endl;
        else std::cout << "Inserted " << a << std::endl;
    }
    std::cout << "Current set size: " << set.GetSize() << std::endl;
    std::cout << "First set element: " << set.GetFirst() << std::endl;
    std::cout << "Last set element: " << set.GetLast() << std::endl;
    
    generator.Reset();
    for (size_t i = 0; i < k_size / 2; i++)
    {
        int a = std::abs(static_cast<int>(generator.Rand()) % 1000);
        if(!set.Contains(a))
        {
            std::cout << "Set had duplicate at i = " << i << " and A = " << a << std::endl;
        }
        else
        {
            set.Remove(a);
            std::cout << "Removed " << a << std::endl;
        }
    }
    std::cout << "Current set size: " << set.GetSize() << std::endl;
    std::cout << "First set element: " << set.GetFirst() << std::endl;
    std::cout << "Last set element: " << set.GetLast() << std::endl;

    return 0;
}
