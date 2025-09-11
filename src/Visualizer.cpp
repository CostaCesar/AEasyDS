#include <iostream>

#include "structs/PQueue.hpp"
#include "Random.hpp"

int main(int argc, char const *argv[])
{
    
    const int k_size = 50;
    auto GetPriority = [](const int& num){
        return num;
    };
    auto Swap = [](int& a, int& b){
        int x = a;
        a = b;
        b = x;
    };
    Aeasyds::PQueue<int> queue(k_size, GetPriority, Swap);
    
    Aeasyds::Random::LFSR_64 generator(1234567890);
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
    
    return 0;
}
