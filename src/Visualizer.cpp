#include <iostream>

#include "structs/PQueue.hpp"
#include "structs/USet.hpp"
#include "Random.hpp"

using std::cout, std::cin, std::cerr, std::endl, std::flush;

constexpr static int k_def_size = 32;
constexpr static int k_rng_seed = 1812;

auto Tabs = [](int level){
    return std::string(level * 2, ' ');
};

/* START Pqueue section */

void Test_PQueue()
{
    const int k_test_size = k_def_size;
    auto generator = Aeasyds::Random::LFSR_64(k_rng_seed);
    
    auto GetPriority = [](const int& num){
        return num;
    };
    auto Swap = [](int& a, int& b){
        int x = a;
        a = b;
        b = x;
    };
    Aeasyds::PQueue<int> queue(k_def_size, GetPriority, Swap);

    auto PrintStats = [&queue](){
        cout << Tabs(1) << "Current size: " << queue.Size() << endl;

        if(!queue.IsEmpty()) cout << Tabs(1) << "Next on queue: " << queue.Get() << endl;
    };

    PrintStats();

    const int k_test_lim_1 = k_test_size / 2;
    cout << Tabs(1) << "<Inserting " << k_test_lim_1 << " elements>" << endl;
    for (int i = 0; i < k_test_lim_1; i++)
    {
        int a = (generator.Rand() % 0xffffffff) % 10;
        queue.Insert(a);
        cout << Tabs(2) << "Inserted " << a << endl;
    }

    PrintStats();
    
    const int k_test_lim_2 = 5;
    cout << Tabs(1) << "<Removing " << k_test_lim_2 << " elements>" << endl;
    while (queue.Size() > k_test_lim_2)
    {
        int b = queue.Remove();
        cout << Tabs(2) << "Removed " << b << endl;
    }
    
    PrintStats();

    cout << Tabs(1) << "<Clearing queue>" << endl;
    queue.Clear();
    
    PrintStats();
    
    const int k_test_lim_3 = queue.Capacity();
    cout << Tabs(1) << "<Filling queue completely with " << k_test_lim_3 << " elements>" << endl;
    for (int i = 0; queue.Size() < k_test_lim_3; i++)
    {
        int a = (generator.Rand() % 0xffffffff) % 10;
        queue.Insert(a);
        cout << Tabs(2) << "Inserted " << a << endl;
    }

    PrintStats();
    
    cout << Tabs(1) << "<Removing all elements from queue, one by one>" << endl;
    while (!queue.IsEmpty())
    {
        int b = queue.Remove();
        cout << Tabs(2) << "Removed " << b << endl;
    }

    PrintStats();
}

void Menu_PQueue()
{
    auto GetPriority = [](const int& num){
        return num;
    };
    auto Swap = [](int& a, int& b){
        int x = a;
        a = b;
        b = x;
    };
    Aeasyds::PQueue<int> queue(k_def_size, GetPriority, Swap);
    
    int option = 0;
    do
    {
        cout << "Priority Queue (int)  \n"
             << "---------------------  \n"
             << "[1] Set maximum size   \n"
             << "[2] Add new item       \n"
             << "[3] Get next item      \n"
             << "[4] Remove next item   \n" 
             << "[5] Get current size   \n"
             << "[6] Run built-in test  \n"
             << "[7] Reset structure    \n"
             << "---------------------  \n"
             << "[0]  Go back"
             << endl;
        
        cout << "> " << flush;
        cin >> option;

    
        int input = -1;
        switch (option)
        {
        case 0:
            break;
        case 1:
            cout << "> New size: " << flush;
            cin >> input;
            queue = Aeasyds::PQueue<int>(input, GetPriority, Swap);
            break;
        case 2:
            cout << "> New item: " << flush;
            cin >> input;
            queue.Insert(input);
            break;
        case 3:
            input = queue.Get();
            cout << "Got element: " << input << endl;
            break;
        case 4:
            input = queue.Remove();
            cout << "Removed element: " << input << endl;
            break;
        case 5:
            input = queue.Size();
            cout << "Current size is: " << input << endl;
            input = queue.Capacity();
            cout << "Max capacity is: " << input << endl;
            break;
        case 6:
            cout << "[Starting test]" << endl;
            Test_PQueue();
            cout << "[Test completed]" << endl;
            break;
        case 7:
            queue.Clear();
            cout << "Queue cleared" << endl;
            break;
        default:
            cerr << "# Invalid option #" << endl;
            break;
        }
    }
    while (option != 0);
}

/* END Pqueue section */

/* Start USet section */

void Test_USet()
{
    const int k_test_size = k_def_size;
    auto generator = Aeasyds::Random::LFSR_64(k_rng_seed);
    
    auto Equals = [](const int& a, const int& b){
        return a == b;
    };
    auto Hash = [](const int& num){
        constexpr static unsigned int k_multp = 0x119de1f3u;
        unsigned int x = num;
        x = ((x >> 16) ^ x) * k_multp;
        x = ((x >> 16) ^ x) * k_multp;
        x = (x >> 16) ^ x;
        return x;
    };
    
    Aeasyds::USet<int> set(Hash, Equals, k_test_size);

    auto PrintStats = [&set](){
        cout << Tabs(1) << "Current set size: " << set.GetSize() << endl;
        cout << Tabs(1) << "Current set capacity: " << set.GetCapacity() << endl;
        
        if(set.IsEmpty()) return; // Avoid exception

        cout << Tabs(1) << "First set element: " << set.GetFirst() << endl;
        cout << Tabs(1) << "Last set element: " << set.GetLast() << endl;
    };

    PrintStats();

    cout << Tabs(1) << "<Trying to insert " << k_test_size << " elements>" << endl;
    for (size_t i = 0; i < k_test_size; i++)
    {
        bool b = false;
        int a = std::abs(static_cast<int>(generator.Rand()) % 1000);
        if(set.Contains(a))
            b = true;
        set.Add(a);
        
        if(b == true)
            cout << Tabs(2) << a << " was already in  the set" << endl;
        else cout << Tabs(2) << "Inserted " << a << endl;
    }

    PrintStats();
    
    generator.Reset();

    const int k_test_lim_1 = k_test_size / 2;
    cout << Tabs(1) << "<Trying to remove " << k_test_lim_1 << " elements>" << endl;
    for (size_t i = 0; i < k_test_lim_1; i++)
    {
        int a = std::abs(static_cast<int>(generator.Rand()) % 1000);
        if(!set.Contains(a))
            cout << Tabs(2) << "Set had duplicate at i = " << i << " and A = " << a << endl;
        else
        {
            set.Remove(a);
            cout << Tabs(2) << "Removed " << a << endl;
        }
    }

    PrintStats();

    const int k_test_lim_2 = k_test_size * 2;
    cout << Tabs(1) << "<Trying to insert " << k_test_lim_2 << " elements>" << endl;
    for (size_t i = 0; i < k_test_lim_2; i++)
    {
        bool b = false;
        int a = std::abs(static_cast<int>(generator.Rand()) % 1000);
        if(set.Contains(a))
            b = true;
        set.Add(a);
        
        if(b == true)
            cout << Tabs(2) << a << " was already in  the set" << endl;
        else cout << Tabs(2) << "Inserted " << a << endl;
    }

    PrintStats();
    
    cout << Tabs(1) << "<Clearing set completely>" << endl;
    set.Clear();

    PrintStats();
}

void Menu_USet()
{
    auto Equals = [](const int& a, const int& b){
        return a == b;
    };
    auto Hash = [](const int& num){
        constexpr static unsigned int k_multp = 0x119de1f3u;
        unsigned int x = num;
        x = ((x >> 16) ^ x) * k_multp;
        x = ((x >> 16) ^ x) * k_multp;
        x = (x >> 16) ^ x;
        return x;
    };
    
    Aeasyds::USet<int> set(Hash, Equals, k_def_size);
    
    int option = 0;
    do
    {
        cout << "*Unique* Set (int)    \n"
             << "---------------------  \n"
             << "[1] Add new item       \n"
             << "[2] Check for item     \n"
             << "[3] Remove an item     \n" 
             << "[4] Get current size   \n"
             << "[5] Get edge elements  \n"
             << "[6] Run built-in test  \n"
             << "[7] Reset structure    \n"
             << "---------------------  \n"
             << "[0] Go back"
             << endl;
        
        cout << "> " << flush;
        cin >> option;

    
        int input = -1;
        switch (option)
        {
        case 0:
            break;
        case 1:
            cout << "> New item: " << flush;
            cin >> input;
            set.Add(input);
            break;
        case 2:
            cout << "> Check item: " << flush;
            cin >> input;
            if(set.Contains(input))
                cout << "Present" << flush;
            else cout << "Missing" << flush;
            break;
        case 3:
            cout << "> Remove item: " << flush;
            cin >> input;
            set.Remove(input);
            break;
        case 4:
            input = set.GetSize();
            cout << "Current size is: " << input << endl;
            input = set.GetCapacity();
            cout << "Max capacity is: " << input << endl;
            break;
        case 5:
            if(!set.IsEmpty())
            {
                input = set.GetFirst();
                cout << "First element: " << input << endl;
                input = set.GetLast();
                cout << "Last element: " << input << endl;
            }
            else cout << "Set is empty" << endl;
            break;
        case 6:
            cout << "[Starting test]" << endl;
            Test_USet();
            cout << "[Test completed]" << endl;
            break;
        case 7:
            set.Clear();
            cout << "Set cleared " << endl;
            break;
        default:
            cerr << "# Invalid option #" << endl;
            break;
        }
    }
    while (option != 0);
}

/* END USet section */

void Section_Strucures()
{
    int option = 0;
    do
    {
        cout << "*Structures*           \n"
             << "---------------------  \n"
             << "[1] Priority Queue     \n"
             << "[2] Set                \n"
             << "---------------------  \n"
             << "[0] Return to main menu"
             << endl;

        cout << "> " << flush;
        cin >> option;

    
        switch (option)
        {
        case 0:
            break;
        case 1:
            Menu_PQueue();
            break;
        case 2:
            Menu_USet();
            break;
        default:
            cout << "# Invalid option #" << endl;
            break;
        }
    } while (option != 0);
}

int main(int argc, char const *argv[])
{
    int option = 0;
    do
    {
        cout << " \n"
             << "  /$$$$$$        /$$$$$$$$                                     /$$$$$$$   /$$$$$$      \n"
             << " /$$__  $$      | $$_____/                                    | $$__  $$ /$$__  $$     \n"
             << "| $$  \\ $$      | $$        /$$$$$$   /$$$$$$$ /$$   /$$      | $$  \\ $$| $$  \\__/    \n"
             << "| $$$$$$$$      | $$$$$    |____  $$ /$$_____/| $$  | $$      | $$  | $$|  $$$$$$      \n"
             << "| $$__  $$      | $$__/     /$$$$$$$|  $$$$$$ | $$  | $$      | $$  | $$ \\____  $$    \n"
             << "| $$  | $$      | $$       /$$__  $$ \\____  $$| $$  | $$      | $$  | $$ /$$  \\ $$    \n"
             << "| $$  | $$      | $$$$$$$$|  $$$$$$$ /$$$$$$$/|  $$$$$$$      | $$$$$$$/|  $$$$$$/     \n"
             << "|__/  |__/      |________/ \\_______/|_______/  \\____  $$      |_______/  \\______/   \n"
             << "                                               /$$  | $$                               \n"
             << "                                              |  $$$$$$/                               \n"
             << "                                               \\______/                               \n"
             << endl;
    
    
        cout << "---------------------  \n"
             << "[1] Structures         \n"
             << "[2] Random Generators  \n"
             << "[3] Sorting Algorithms \n" 
             << "[4] Mathematic Solvers \n" 
             << "---------------------  \n"
             << "[0] Quit"
             << endl;

        cout << "> " << flush;
        cin >> option;


        switch (option)
        {
        case 0:
            break;
        case 1:
            Section_Strucures();
            break;
        case 2:
        case 3:
        case 4:
            cerr << "# Not implemented yet #" << endl;
            break;
        default:
            cerr << "# Invalid option #" << endl;
            break;
        }

    } while (option != 0);

    return 0;
}
