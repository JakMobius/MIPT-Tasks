#include <iostream>
#include "logger-collector.hpp"
#include "dot-collector.hpp"

template<typename Type>
struct my_remove_reference { Type value; };

template<typename Type>
struct my_remove_reference<Type&> { Type value; };

template<typename Type>
struct my_remove_reference<Type&&> { Type value; };

template <class Type>
using my_remove_reference_t = typename my_remove_reference<Type>::value;

template<typename T>
my_remove_reference_t<T>&& move(T&& a) {
    return static_cast<my_remove_reference_t<T>&&>(a);
}

template<typename T>
T&& forward(T&& a) {
    return static_cast<T&&>(a);
}

/*
template<typename T>
void swap(T& collector, SmartValue<int>& a, SmartValue<int>& b) {
    COLLECTOR_FUNC(collector)
    std::swap(a, b);
}

SmartValue<int> foo(SmartValue<int> value, SmartValue<int> value2) {
    return value + value2;
}

template<typename T>
void experiment(T& collector) {
    COLLECTOR_FUNC(collector)
    SMART_VALUE(collector, a, 10);
    SMART_VALUE(collector, b, 20);
    SMART_VALUE(collector, c, 30);
    SMART_VALUE(collector, d, 2);

    c = foo(a * b, c);

    std::cout << a.get_value();
}

int main() {
//    LoggedCollector<int> collector { "log.html" };
    DotCollector<int> collector { "log.dot" };

    experiment(collector);

    collector.complete();

    return 0;
}

int main() {
    LoggedCollector<std::string> collector { "log.html" };
    COLLECTOR_FUNC(collector)

    SMART_VALUE(collector, a, "hubba");
    SMART_VALUE(collector, b, "bubba");
    SMART_VALUE(collector, c, "wubba");

    c = a + b;
    b = b + c;

    std::cout << b.get_value();

    return 0;
} */

template<typename T>
void perform_revolution(T&& subject) {
    typedef typename std::remove_reference<T>::type value_type;

    value_type moved(std::forward<T>(subject));
}

template<typename C, typename T>
void forward_call(C& collector, T&& subject) {
    COLLECTOR_FUNC(collector)
    perform_revolution(forward<T>(subject));
}

template<typename C, typename T>
void move_call(C& collector, T& subject) {
    COLLECTOR_FUNC(collector)
    perform_revolution(move(subject));
}

int main() {

    DotCollector<std::string> collector { "log.dot" };
    {
//        SMART_VALUE(collector, a, "normal text");

        std::vector<SmartValue<std::string>> vector;
        vector.reserve(99);

//        const auto value = SmartValue<std::string> {&collector, std::string("20"), std::string("push_back")};

        vector.emplace_back(&collector, "10", "emplace_back 1");
        vector.emplace_back(&collector, "20", "emplace_back 2");
        vector.emplace_back(&collector, "30", "emplace_back 3");
        vector.emplace_back(&collector, "40", "emplace_back 4");
//        vector.push_back(value);
//        vector.push_back({&collector, std::string("20"), std::string("push_back")});

//        std::cout << "text before call = " << QuotedPrinter(a.get_value()) << "\n";
//        move_call(collector, a);
//        std::cout << "text after call = " << QuotedPrinter(a.get_value()) << "\n";
//        if(a.get_value().empty()) {
//            std::cout << "Меня украли 😭😭😭😭😭\n";
//        }
    }

    collector.complete();

    return 0;
}