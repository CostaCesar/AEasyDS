#include <iostream>

#include "structs/PQueue.hpp"

int main(int argc, char const *argv[])
{
    srand(123456789);
    
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

    for (int i = 0; i < k_size / 2; i++)
    {
        int a = rand() % 10;
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
