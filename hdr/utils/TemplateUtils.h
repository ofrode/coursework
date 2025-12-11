#ifndef TEMPLATEUTILS_H
#define TEMPLATEUTILS_H

#include <vector>
#include <algorithm>
#include <ranges>
#include <iterator>

// Шаблонная функция для поиска элемента в контейнере (C++20 ranges)
template<typename Container, typename Value>
auto findInContainer(const Container& container, const Value& value) {
    return std::ranges::find(container, value);
}

// Шаблонная функция для подсчета элементов (C++20 ranges)
template<typename Container, typename Predicate>
int countIf(const Container& container, Predicate pred) {
    return static_cast<int>(std::ranges::count_if(container, pred));
}

// Шаблонный класс для работы с итераторами
template<typename T>
class ContainerIterator {
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    
    explicit ContainerIterator(std::vector<T>& container) : container_(container) {}
    
    iterator begin() { return std::begin(container_); }
    iterator end() { return std::end(container_); }
    const_iterator begin() const { return std::cbegin(container_); }
    const_iterator end() const { return std::cend(container_); }
    const_iterator cbegin() const { return std::cbegin(container_); }
    const_iterator cend() const { return std::cend(container_); }
    
private:
    std::vector<T>& container_;
};

#endif // TEMPLATEUTILS_H

