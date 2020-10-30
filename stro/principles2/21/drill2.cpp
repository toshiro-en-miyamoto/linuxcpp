#include <iostream>
#include <map>

int main()
{
    std::cout << "1. Define a map<string, int>" << std::endl;
    std::map<std::string, int> msi{};

    std::cout << "2. Insert (name, value) pairs" << std::endl;
    msi["lecture"] = 30;
    msi["translation"] = 20;
    msi["programming"] = 30;
    msi["management"] = 50;
    msi["system design"] = 40;

    std::cout << "3. cout << the (name, value) pairs" << std::endl;
    for(const auto& si : msi)
        std::cout << si.first << " : " << si.second << std::endl;

    std::cout << "4. Erase the 'lecture' pair" << std::endl;
    msi.erase("lecture");
    for(const auto& si : msi)
        std::cout << si.first << " : " << si.second << std::endl;

    return 0;
}