#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Item {
    std::string name;
    int iid;
    double value;
};

std::ostream& operator<<(std::ostream& os, const Item& item)
{
    return os
        << '('
            << item.name
            << ','
            << item.iid
            << ','
            << item.value
        << ')';
}

std::istream& operator>>(std::istream& is, Item& item)
{
    std::string name;
    int iid;
    double value;

    char paren1, delim1, delim2, paren2;
    is >> paren1 >> name >> delim1 >> iid >> delim2 >> value >> paren2;
    if(!is) return is;
    if(paren1!='(' || delim1!=',' || delim2!=',' || paren2!=')') {
        is.clear(std::ios_base::failbit);
        return is;
    }
    item = Item{name, iid, value};

    return is;
}

int main()
{
    std::ifstream ifile("drill1.txt");
    if(!ifile) {
        std::cerr << "Failed to open [drill1.txt]" << std::endl;
        return -1;
    }

    std::vector<Item> vi;
    Item item{};

    std::cout << "1. fill vector<Item> with items from the file" << std::endl;
    while(ifile >> item)
        vi.push_back(item);
    for(auto item : vi) std::cout << item << std::endl;

    std::cout << "2. sort vector<Item> by name" << std::endl;
    std::sort(std::begin(vi), std::end(vi),
        [](const Item& a, const Item& b){ return a.name < b.name; }
    );
    for(auto item : vi) std::cout << item << std::endl;

    std::cout << "3. sort vector<Item> by iid" << std::endl;
    std::sort(std::begin(vi), std::end(vi),
        [](const Item& a, const Item& b){ return a.iid < b.iid; }
    );
    for(auto item : vi) std::cout << item << std::endl;

    std::cout << "4. sort vector<Item> by value in descending" << std::endl;
    std::sort(std::begin(vi), std::end(vi),
        [](const Item& a, const Item& b){ return a.value > b.value; }
    );
    for(auto item : vi) std::cout << item << std::endl;

    std::cout << "5. insert two items" << std::endl;
    vi.push_back(Item{"horse shoe",99,12.34});
    vi.push_back(Item{"Canon S400", 9988,499.95});
    for(auto item : vi) std::cout << item << std::endl;

    std::cout << "6. remove an item identified by name : ";
    std::string name;
    while(std::cin >> name) {
        for(auto it = vi.begin(); it != vi.end(); )
            it->name == name ? it = vi.erase(it) : ++it;
        std::cout << "6. remove an item identified by name : ";
    }
    std::cout << "[quit]" << std::endl;
    for(auto item : vi) std::cout << item << std::endl;


    return 0;
}