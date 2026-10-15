#include <iostream>
#include <string>

void checkComponent(const std::string& name, double temperature)
{
    std::cout << "Component: " << name << std::endl;
    std::cout << "Temperature: " << temperature << std::endl;

    if (temperature > 80.0)
    {
        std::cout << "Status: WARNING" << std::endl;
    }
    else
    {
        std::cout << "Status: NORMAL" << std::endl;
    }
}

int main()
{
    checkComponent("Training Controller", 45.5);
    return 0;
}