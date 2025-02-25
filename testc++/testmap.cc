#include <iostream>
#include <map>

int main()
{

    std::map<std::string, int> val = {{"a", 1}, {"b", 2}};
    for(auto it = val.begin(); it != val.end(); ++it)  {
        std::cout << "it's key = " << it->first << " it'value" << it->second << std::endl;
    }
    return 0;
}

