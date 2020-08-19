// XTuple.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

template<typename ... Types>
struct xtuple
{
    static constexpr size_t N = 0;
};

template<typename T, typename ... Types>
struct xtuple<T, Types...> : xtuple<Types...>
{
    static constexpr size_t N = xtuple<Types...>::N + 1;
    T _value;
    xtuple(const T& value, const Types& ... args) : _value(value), xtuple<Types...>(args...) {}
};

template<size_t N, typename TP>
struct xtuple_element
{
};

template<typename T, typename ... Types>
struct xtuple_element<0, xtuple<T, Types...>>
{
    using type = T;
    using typeTrait = xtuple<T, Types...>;
};

template<size_t N, typename T, typename ... Types>
struct xtuple_element<N, xtuple<T, Types...>>
{
    using type = typename xtuple_element<N - 1, xtuple<Types...>>::type;
    using typeTrait = typename xtuple_element<N - 1, xtuple<Types...>>::typeTrait;
};

template<size_t N, typename TP>
typename xtuple_element<N, TP>::type& xget(TP& tp)
{
    return static_cast<typename xtuple_element<N, TP>::typeTrait&>(tp)._value;
}

template<size_t N, typename TP>
typename std::enable_if<N == 0>::type xprint_impl(TP& tp)
{
    std::cout << xget<N>(tp) << std::endl;
}

template<size_t N, typename TP>
typename std::enable_if<N != 0>::type xprint_impl(TP& tp)
{
    xprint_impl<N - 1>(tp);
    std::cout << xget<N>(tp) << std::endl;
}

template<typename TP>
void xprint(TP& tp)
{
    xprint_impl<TP::N - 1>(tp);
}


int main()
{
    xtuple<int32_t, double, bool> tp(10, 3.4, true);

    std::cout << "value" << std::endl;
    std::cout << tp._value << std::endl;

    std::cout << "values" << std::endl;
    std::cout << static_cast<xtuple<int32_t, double, bool>&>(tp)._value << std::endl;
    std::cout << static_cast<xtuple<         double, bool>&>(tp)._value << std::endl;
    std::cout << static_cast<xtuple<                 bool>&>(tp)._value << std::endl;

    std::cout << "param count" << std::endl;
    std::cout << xtuple<int32_t, double, bool>::N << std::endl;
    std::cout << xtuple<         double, bool>::N << std::endl;
    std::cout << xtuple<                 bool>::N << std::endl;

    std::cout << "xget" << std::endl;
    std::cout << xget<0>(tp) << std::endl;
    std::cout << xget<1>(tp) << std::endl;
    std::cout << xget<2>(tp) << std::endl;

    xget<0>(tp) = 22;
    xget<1>(tp) = 3.14;
    xget<2>(tp) = false;
    std::cout << "xget set" << std::endl;
    std::cout << xget<0>(tp) << std::endl;
    std::cout << xget<1>(tp) << std::endl;
    std::cout << xget<2>(tp) << std::endl;

    std::cout << "print" << std::endl;
    xprint(tp);

    return 0;
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
