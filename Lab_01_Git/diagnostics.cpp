#include <iostream>
#include <string>

void diagnoseComponent(const std::string& name, int errorCode)
{
    std::cout << "Component: " << name << std::endl;

    if (errorCode == 0)
    {
        std::cout << "Status: OK" << std::endl;
    }
    else if (errorCode == 1)
    {
        std::cout << "Status: WARNING" << std::endl;
    }
    else
    {
        std::cout << "Status: ERROR" << std::endl;
    }
}

int main()
{
    diagnoseComponent("Training Controller", 1);
    return 0;
}