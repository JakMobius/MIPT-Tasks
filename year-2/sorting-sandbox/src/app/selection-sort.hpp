#pragma once

#include <algorithm>
#include <iterator>

template<typename Iterator, typename Comparator>
void selection_sort(Iterator begin, Iterator end, Comparator comparator) {
    for (; begin != end; ++begin) {

        auto min = std::min_element(begin, end, comparator);

        if (comparator(*min, *begin)) {
            std::iter_swap(min, begin);
        }
    }
}
template<typename Iterator>
void selection_sort(Iterator begin, Iterator end) {
    typedef typename std::iterator_traits<Iterator>::value_type value_type;

    selection_sort(begin, end, std::less<value_type>());
}