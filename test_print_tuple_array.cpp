/* This is a sample for template and tuple usage
 *
 * Author: Cindy Liu
 * Date: Mar. 13, 2024
 * Note: If you use clang compiler, you should add -std=c++20 option
 */

#include <array>
#include <iostream>
#include <utility>
#include <tuple>
 
// debugging aid
template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    ((std::cout << ints << ' '), ...);
    std::cout << '\n';
}
 
// convert array into a tuple
template<typename Array, std::size_t... I>
auto a2t_impl(const Array& a, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}
 
template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
auto a2t(const std::array<T, N>& a)
{
    return a2t_impl(a, Indices{});
}
 
// pretty-print a tuple
template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch, Tr>& os,
                      const Tuple& t,
                      std::index_sequence<Is...>)
{
    ((os << (Is == 0? "" : ", ") << std::get<Is>(t)), ...);
}
 
template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os,
                 const std::tuple<Args...>& t)
{
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
}

template<typename... Args>
void print_array(Args... args)
{ ((std::cout << args << " "), ...);
std::cout << std::endl;
}

template<class Ch, class Tr, typename T, std::size_t N>
auto&  operator<<(std::basic_ostream<Ch, Tr>& os, const std::array<T, N> & arr)
{ [&]<std::size_t... I>(std::index_sequence<I...>)
  { print_array(arr[I]...); }
   (std::make_index_sequence<N>()); 
    return os;}

int main() {
    print_sequence(std::integer_sequence<unsigned, 9, 2, 5, 1, 9, 1, 6>{});
    print_sequence(std::make_integer_sequence<int, 20>{});
    print_sequence(std::make_index_sequence<10>{});
    print_sequence(std::index_sequence_for<float, std::iostream, char>{});
 
    std::array<int, 4> array = {1, 2, 3, 4};
 
    // convert an array into a tuple
    auto tuple = a2t(array);
    static_assert(std::is_same_v<decltype(tuple),
                  std::tuple<int, int, int, int>>, "");
                                 
    // print the array to cout
    std::cout << "The Array: " << array << '\n';
    // print the tuple to cout
    std::cout << "The tuple: " << tuple << '\n';

    return 0;
}
