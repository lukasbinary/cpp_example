// VoidT.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <type_traits>

class CustomType
{
public:
    const char* ToString()
    {
        return "CustomType";
    }
};

template<typename, typename = std::void_t<>>
struct IsCustomType : std::false_type {};

template<typename T>
struct IsCustomType<T, std::void_t<decltype(std::declval<T>().ToString())>> : std::true_type {};

template<typename T>
void WriteArgs(T&& first)
{
    if constexpr (std::is_integral_v<T>)
    {
        std::cout << "integral : " << first << std::endl;
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
        std::cout << "floating_point : " << first << std::endl;
    }
    else if constexpr (IsCustomType<T>::value)
    {
        std::cout << "custom : " << first.ToString() << std::endl;
    }
    else
    {
        std::cout << "etc : " << first << std::endl;
    }
}

template<typename T, typename ... Args>
void WriteArgs(T&& first, Args&& ... args)
{
    WriteArgs(std::forward<T>(first));
    WriteArgs(std::forward<Args>(args)...);
}

int main()
{
    WriteArgs(1, CustomType(), "test", 9.1f, 3.14);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
