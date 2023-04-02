/*
    Project: LRU Cache Simulator
    Code Author: Vraj Patel
*/

#include <iostream>
#include <unordered_map>
#include <windows.h>
#include <time.h>

using namespace std;

int numrandom()
{
    srand(time(NULL));
    int rnum = (rand() % 255);
    return rnum;
}

class TextAttr
{
    friend std::ostream &operator<<(std::ostream &out, TextAttr attr)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr.value);
        return out;
    }

public:
    explicit TextAttr(WORD attributes) : value(attributes) {}

private:
    WORD value;
};

#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

void fontsize(int j)
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0; // Width of each character in the font
    cfi.dwFontSize.Y = j; // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

class Node
{
public:
    int k;
    int val;
    Node *prev;
    Node *next;

    Node(int key, int value)
    {
        k = key;
        val = value;
        prev = NULL;
        next = NULL;
    }
};

class LRUCache
{
public:
    LRUCache(int capacity)
    {
        cap = capacity;

        left = new Node(0, 0);
        right = new Node(0, 0);

        left->next = right;
        right->prev = left;
    }

    int get(int key)
    {
        if (cache.find(key) != cache.end())
        {
            remove(cache[key]);
            insert(cache[key]);
            return cache[key]->val;
        }

        put(key, numrandom());
        return -1;
    }

    void put(int key, int value)
    {
        if (cache.find(key) != cache.end())
        {
            remove(cache[key]);
        }
        cache[key] = new Node(key, value);
        insert(cache[key]);

        if (cache.size() > cap)
        {
            // remove from list & delete LRU from map
            Node *lru = left->next;
            remove(lru);
            cache.erase(lru->k);
        }
    }

    void display()
    {
        unordered_map<int, Node *>::iterator p;
        cout << "\n===> Cache Contents\n";
        cout << "Top to Bottom === MRU to LRU\n";
        int i = 0;
        Node *mru = right->prev;
        while (mru->prev)
        {
            cout << "LRU Index = " << i << " | Address: " << mru->k << " | Data: " << mru->val << endl;
            mru = mru->prev;
            i += 1;
        }
    }

private:
    int cap;
    unordered_map<int, Node *> cache; // {key -> node}
    Node *left;
    Node *right;

    // remove node from list
    void remove(Node *node)
    {
        Node *prev = node->prev;
        Node *next = node->next;

        prev->next = next;
        next->prev = prev;
    }

    // insert node at right
    void insert(Node *node)
    {
        Node *prev = right->prev;
        Node *next = right;

        prev->next = node;
        next->prev = node;

        node->prev = prev;
        node->next = next;
    }
};

void cachedisplay(LRUCache lRUCache, int capacity, float cache_access_time, float main_access_time)
{
    int requests = 0, hits = 0, access, op = 0, address;
    float hit_rate = 0, average_access_time = 0;

    while (1)
    {
        system("cls");
        std::cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_RED);
        cout << "=====>     LRU Cache Simulator    <=====" << endl;
        cout << "=====>        Creator: Vraj       <=====" << endl << endl;

        cout << "===> Default Parametrs" << endl;
        cout << "Cache Capacity:            " << capacity << " Blocks" << endl;
        cout << "Cache Memory access time:  ";
        printf("%.2f", cache_access_time);
        cout << " ns" << endl;
        cout << "Main Memory access time:   ";
        printf("%.2f", main_access_time);
        cout << " ns" << endl;

        std::cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        cout << "\n===> Cache Analysis" << endl;
        cout << "Requests:             " << requests << endl;
        cout << "Hits:                 " << hits << endl;
        cout << "Hit Rate:             ";
        printf("%.2f", hit_rate); 
        cout << "%" << endl;
        cout << "Average Access Time:  ";
        printf("%.2f", average_access_time); 
        cout << "ns" << endl;

        std::cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
        lRUCache.display();

        std::cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        cout << "\n===> Available Operations\n";
        cout << "To put block in cache enter 1\n";
        cout << "To get block from cache enter 2\n";
        cout << "To exit enter any other character\n";

        std::cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);

        if (op==1)
        {
            cout << "\n===> Operation Result\n";
            cout << "Data Fetched from Main Memory\n";
        }
        else if (op == 2 && access != -1)
        {
            cout << "\n===> Operation Result\n";
            cout << "Data Available in Cache!\n";
        }
        else if (op == 2)
        {
            cout << "\n===> Operation Result\n";
            cout << "Address Not Found!\n";
            cout << "Accessed from Main Memory\n";
        }

        cout << "\nEnter Operation: ";
        cin >> op;
        if (op == 1)
        {
            int data;
            cout << "Enter the address: ";
            cin >> address;
            data = numrandom();
            lRUCache.put(address, data);
        }
        else if (op == 2)
        {
            requests++;
            cout << "Enter the address to fetch the data: ";
            cin >> address;
            access = lRUCache.get(address);
            if (access != -1)
                hits++;
            hit_rate = ((float)hits / requests) * 100;
            average_access_time = (hits * cache_access_time + (requests - hits) * main_access_time) / requests;
        }
        else
            return;
    }
}

void LRUinit()
{
    fontsize(20);
    std::cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
    cout << "=====>     LRU Cache Simulator    <=====" << endl;
    cout << "=====>        Creator: Vraj       <=====" << endl << endl;
    
    int capacity;
    float main_access_time, cache_access_time;
    std::cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_WHITE);
    cout << "Enter the Cache Capacity (in Blocks): ";
    cin >> capacity;
    cout << "Enter the Cache memory access time (in ns): ";
    cin >> cache_access_time;
    cout << "Enter the Main memory access time (in ns): ";
    cin >> main_access_time;
    
    LRUCache lRUCache = LRUCache(capacity);
    cachedisplay(lRUCache, capacity, cache_access_time, main_access_time);
}

int main()
{
    system("title LRU Cache Simulator");
    LRUinit();
    return 0;
}