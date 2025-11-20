#ifndef TEMPLATEUTILS_H
#define TEMPLATEUTILS_H

#include <vector>
#include <algorithm>
#include <iterator>

// Шаблонная функция для поиска элемента в контейнере
template<typename Container, typename Value>
typename Container::const_iterator findInContainer(const Container& container, const Value& value) {
    return std::find(container.begin(), container.end(), value);
}

// Шаблонная функция для подсчета элементов
template<typename Container, typename Predicate>
int countIf(const Container& container, Predicate pred) {
    return static_cast<int>(std::count_if(container.begin(), container.end(), pred));
}

// Шаблонный класс для работы с итераторами
template<typename T>
class ContainerIterator {
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    
    ContainerIterator(std::vector<T>& container) : container_(container) {}
    
    iterator begin() { return container_.begin(); }
    iterator end() { return container_.end(); }
    const_iterator begin() const { return container_.begin(); }
    const_iterator end() const { return container_.end(); }
    const_iterator cbegin() const { return container_.cbegin(); }
    const_iterator cend() const { return container_.cend(); }
    
private:
    std::vector<T>& container_;
};

#endif // TEMPLATEUTILS_H

