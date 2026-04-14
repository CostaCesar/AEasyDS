#include <iostream>

#include "structs/PQueue.hpp"
#include "structs/USet.hpp"
#include "structs/Map.hpp"
#include "Random.hpp"
#include "StringFind.hpp"

using std::cout, std::cin, std::cerr, std::endl, std::flush, std::string;

constexpr static int k_def_size = 32;
constexpr static int k_rng_seed = 1812;

auto Tabs = [](int level){
    return string(level * 2, ' ');
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
        cout << Tabs(1) << "Current size: " << queue.GetSize() << endl;

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
    while (queue.GetSize() > k_test_lim_2)
    {
        int b = queue.Remove();
        cout << Tabs(2) << "Removed " << b << endl;
    }
    
    PrintStats();

    cout << Tabs(1) << "<Clearing queue>" << endl;
    queue.Clear();
    
    PrintStats();
    
    const int k_test_lim_3 = queue.GetCapacity();
    cout << Tabs(1) << "<Filling queue completely with " << k_test_lim_3 << " elements>" << endl;
    while (queue.GetSize() < k_test_lim_3)
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
            input = queue.GetSize();
            cout << "Current size is: " << input << endl;
            input = queue.GetCapacity();
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

/* START Map section */

void Test_Map()
{
    const int k_test_size = k_def_size;
    auto generator = Aeasyds::Random::LFSR_64(k_rng_seed);

    auto Hash = [](const string& word){
        unsigned int x = std::hash<string>{}(word);
        return x;
    };
    
    Aeasyds::Map<string, int> map(Hash, k_test_size);

    auto PrintStats = [&map](){
        cout << Tabs(1) << "Current set size: " << map.GetSize() << endl;
        cout << Tabs(1) << "Current set capacity: " << map.GetCapacity() << endl;
    };

    PrintStats();

    const int k_test_lim_1 = k_test_size;
    cout << Tabs(1) << "<Inserting " << k_test_lim_1 << " elements>" << endl;
    for (int i = 0; i < k_test_lim_1; i++)
    {
        auto key = string(i, '*');
        auto value = generator.Rand() % 20;

        if(map.Contains(key))
            cout << Tabs(2) << "Updating <" << key << "> with value " << value << endl;
        else cout << Tabs(2) << "Adding new <" << key << "> with value " << value << endl;

        map.AddElement(key, value);
    }
    
    PrintStats();

    const int k_test_lim_2 = k_test_size / 2;
    cout << Tabs(1) << "<Removing " << k_test_lim_2 << " elements, at every other>" << endl;
    for (int i = 0; i < k_test_lim_2; i++)
    {
        auto key = string(i * 2, '*');
        if(!map.Contains(key)) continue;

        auto value = map.GetElement(key);
        cout << Tabs(2) << "Removed <" << key << "> with value " << value << endl;
        map.RemoveElement(key);
    }
    
    PrintStats();

    cout << Tabs(1) << "<Clearing map>" << endl;
    map.Clear();
    
    PrintStats();

    const int k_test_lim_3 = k_test_size * 2;
    cout << Tabs(1) << "<Inserting " << k_test_lim_3 << " elements>" << endl;
    for (int i = 0; i < k_test_lim_3; i++)
    {
        auto key = string(i, '#');
        auto value = i % 5;

        if(map.Contains(key))
            cout << Tabs(2) << "Updating <" << key << "> with value " << value << endl;
        else cout << Tabs(2) << "Adding new <" << key << "> with value " << value << endl;

        map.AddElement(key, value);
    }
    
    PrintStats();

    const int k_test_lim_4 = map.GetSize();
    cout << Tabs(1) << "<Removing " << k_test_lim_4 << " elements, one by one>" << endl;
    for (int i = k_test_lim_4 - 1; !map.IsEmpty(); i--)
    {
        auto key = string(i, '#');
        if(!map.Contains(key)) continue;
        
        auto value = map.GetElement(key);
        cout << Tabs(2) << "Removed <" << key << "> with value " << value << endl;
        map.RemoveElement(key);
    }
    
    PrintStats();
}

void Menu_Map()
{
    auto Hash = [](const string& word){
        unsigned int x = std::hash<string>{}(word);
        return x;
    };
    
    Aeasyds::Map<string, int> map(Hash, k_def_size);
    
    int option = 0;
    do
    {
        cout << " Map (string -> int)    \n"
             << "---------------------  \n"
             << "[1] Add new item       \n"
             << "[2] Check for item     \n"
             << "[3] Remove an item     \n" 
             << "[4] Get current size   \n"
             << "[5] Run built-in test  \n"
             << "[6] Reset structure    \n"
             << "---------------------  \n"
             << "[0] Go back"
             << endl;
        
        cout << "> " << flush;
        cin >> option;

    
        int input = -1;
        std::string buffer;
        switch (option)
        {
        case 0:
            break;
        case 1:
            cout << "> New item key: " << flush;
            cin >> buffer;
            cout << "> New item value: " << flush;
            cin >> input;
            map.AddElement(buffer, input);
            break;
        case 2:
            cout << "> Check item key: " << flush;
            cin >> buffer;
            if(map.Contains(buffer))
                cout << "Present | Value: " << map.GetElement(buffer) << flush;
            else cout << "Missing" << flush;
            break;
        case 3:
            cout << "> Remove item key: " << flush;
            cin >> buffer;
            if(map.Contains(buffer))
                map.RemoveElement(buffer);
            else cout << "Value not present" << endl;
            break;
        case 4:
            input = map.GetSize();
            cout << "Current size is: " << input << endl;
            input = map.GetCapacity();
            cout << "Max capacity is: " << input << endl;
            break;
        case 5:
            cout << "[Starting test]" << endl;
            Test_Map();
            cout << "[Test completed]" << endl;
            break;
        case 6:
            map.Clear();
            cout << "Set cleared " << endl;
            break;
        default:
            cerr << "# Invalid option #" << endl;
            break;
        }
    }
    while (option != 0);    
}

/* END Map section */

/* START String section */

void Menu_StringMatch(Aeasyds::StringFind::I_Find* searcher)
{
    if(searcher == nullptr) return;
    
    int option = 0;
    string text = "", pattern = "";
    std::vector<size_t> matches = {};

    do
    {
        cout << "*String Match Menu *   \n"
             << "---------------------  \n"
             << "[1] Set text           \n"
             << "[2] View text          \n"
             << "[3] Set pattern        \n"
             << "[4] View pattern       \n"
             << "[5] Find matches       \n"
             << "[6] Run built-in test  \n"
             << "---------------------  \n"
             << "[0] Go back            "
             << endl;

        cout << "> " << flush;
        cin >> option;
    
        switch (option)
        {
        case 0:
            break;
        case 1:
            cout << "> Text: " << flush;
            cin.ignore(UINT32_MAX, '\n');
            std::getline(cin, text);
            searcher->SetupText(text);
            cout << endl;
            break;
        case 2:
            if(text.empty())
                cerr << "# Text is empty #" << endl;
            else cout << "\n" << text << endl;
            break; 
        case 3:
            cout << "> Pattern: " << flush;
            cin.ignore(UINT32_MAX, '\n');
            std::getline(cin, pattern);
            cout << endl;
            break;
        case 4:
            if(text.empty())
                cerr << "# Pattern is empty #" << endl;
            else cout << "\n" << pattern << endl;
            break; 
        case 5:
            matches = searcher->Find(pattern, text);
            if(matches.size() > 0)
            {
                cout << "Found " << matches.size() << " references ending at indexes:" << "\n";
                for (auto &&i : matches) cout << " " << i;
                cout << endl;
            }
            else cout << "No references of \"" << pattern << "\" found!" << endl;
            break;
        case 6:
            text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum";
            pattern = "in";
            matches = searcher->Find(pattern, text);
            if(matches.size() != 7)
            {
                cerr << "# Test failed: found" << matches.size() << " references of \"" << pattern << "\" instead of " << 7 << " #" << endl;
                break;
            }
            else cout << "[Test completed with success]" << endl;
            break;
        default:
            cout << "# Invalid option #" << endl;
            break;
        }
    } while (option != 0);
}

/* END String section */

void Section_Structures()
{
    int option = 0;
    do
    {
        cout << "*Structures*           \n"
             << "---------------------  \n"
             << "[1] Priority Queue     \n"
             << "[2] Set                \n"
             << "[3] Map / Dictionary   \n"
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
        case 3:
            Menu_Map();
            break;
        default:
            cout << "# Invalid option #" << endl;
            break;
        }
    } while (option != 0);
}

void Section_StringMatchers()
{
    int option = 0;
    do
    {
        Aeasyds::StringFind::I_Find* searcher = nullptr;

        cout << "*Match strategies*     \n"
             << "---------------------  \n"
             << "[1] ShiftAnd           \n"
             << "[2] Horspool           \n"
             << "[3] KMP algorithm      \n"
             << "[4] Trie               \n"
             << "---------------------  \n"
             << "[0] Return to main menu"
             << endl;

        cout << "> " << flush;
        cin >> option;
    
        switch (option)
        {
        case 0:
            if(searcher) delete searcher;
            return;
        case 1:
            searcher = new Aeasyds::StringFind::ShiftAnd();
            break;
        case 2:
            searcher = new Aeasyds::StringFind::Horspool();
            break;
        case 3:
        case 4:
            cerr << "# Not implemented yet #" << endl;
            continue;
        default:
            cout << "# Invalid option #" << endl;
            continue;
        }

        if(searcher)
        {
            Menu_StringMatch(searcher);
            delete searcher;
            searcher = nullptr;
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
             << "[5] String Matchers    \n" 
             << "---------------------  \n"
             << "[0] Quit"
             << endl;

        cout << "> " << flush;
        cin >> option;

        Aeasyds::StringFind::ShiftAnd foo;
        switch (option)
        {
        case 0:
            break;
        case 1:
            Section_Structures();
            break;
        case 5:
            Section_StringMatchers();
            foo.Find("sudo make sudo clean", "sudo make sudo make sudo clean sudo sudo make sudo clean");
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
