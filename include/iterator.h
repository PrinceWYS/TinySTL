#ifndef INCLUDE_ITERATOR_H__
#define INCLUDE_ITERATOR_H__

#include <cstddef>

namespace tinystl
{
// 五种迭代器类型
struct input_iterator_tag { };                                              // 输入迭代器 允许读 只能 ++
struct output_iterator_tag { };                                             // 输出迭代器 允许写入 只能 ++
struct forward_iterator_tag : public input_iterator_tag { };                // 前向迭代器 允许读写 只能 ++
struct bidirectional_iterator_tag : public forward_iterator_tag { };        // 双向迭代器 允许读写 可以 ++, --
struct random_access_iterator_tag : public bidirectional_iterator_tag { };  // 随机访问迭代器 可以 ++, --, +n, -n

// 迭代器模板
template<typename Catogory, typename T, typename Distance = ptrdiff_t,
         typename Pointer = T*, typename Reference = T&>
struct iterator {
    using iterator_catogory = Catogory;     // 迭代器类型
    using value_type        = T;            // 迭代器指向的对象类型
    using difference_type   = Distance;     // 迭代器之间的距离
    using pointer           = Pointer;      // 迭代器指向元素的指针类型
    using reference         = Reference;    // 迭代器指向元素的引用类型
};

// 迭代器的 traits
template<typename Iterator>
struct iterator_traits {
    using value_type        = typename Iterator::value_type;
    using difference_type   = typename Iterator::difference_type;
    using pointer           = typename Iterator::pointer;
    using reference         = typename Iterator::reference;
    using iterator_catogory = typename Iterator::iterator_catogory;
};

// 偏特化
template<typename T>
struct iterator_traits<T*> {
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;
    using iterator_catogory = random_access_iterator_tag; // 指针是随机访问迭代器
};

template<typename T>
struct iterator_traits<const T*> {
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = const T*;
    using reference         = const T&;
    using iterator_catogory = random_access_iterator_tag; // 指针是随机访问迭代器
};

template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_catogory iterator_category(const Iterator&) {
    using catogory = typename iterator_traits<Iterator>::iterator_catogory();
    return catogory;
};

template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
};

template<typename Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
};

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance_aux(InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first, ++n) { }
    return n;
}

template<typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
distance_aux(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
    return last - first;
}

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    return distance_aux(first, last, iterator_category(first));
}

template<typename InputIterator, typename Distance>
inline void advance_aux(InputIterator& iter, const Distance& n, input_iterator_tag) {
    for (; n != 0; --n, ++iter) { }
}

template<typename BidirectionalIterator, typename Distance>
inline void advance_aux(BidirectionalIterator& iter, const Distance& n, bidirectional_iterator_tag) {
    if (n > 0) {
        for (; n != 0; --n, ++iter) { }
    } else {
        for (; n != 0; ++n, --iter) { }
    }
}

template<typename RandomAccessIterator, typename Distance>
inline void advance_aux(RandomAccessIterator& iter, const Distance& n, random_access_iterator_tag) {
    iter += n;
}

template<typename InputIterator, typename Distance>
inline void advance(InputIterator& iter, const Distance& n) {
    advance_aux(iter, n, iterator_category(iter));
}

// 反向迭代器实现
template<typename RandomAccessIterator, typename T,
         typename Reference = T&, typename Distance = ptrdiff_t>
class reverse_iterator {
protected:
    using self = reverse_iterator<RandomAccessIterator, T>;
    RandomAccessIterator current;
public:
    using iterator_catogory = random_access_iterator_tag
    using value_type        = T;
    using difference_type   = Distance;
    using pointer           = T*;
    using reference         = Reference;

    reverse_iterator() {}
    explicit reverse_iterator(RandomAccessIterator iter) : current(iter) { }
    RandomAccessIterator base() const { return current; }

    // 操作符重载
    Reference operator*() const { return *(current - 1); }
    pointer operator->() const { return &(operator*()); }
    self& operator++() {
        --current;
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        --current;
        return tmp;
    }
    self& operator--() {
        ++current;
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        ++current;
        return tmp;
    }
    self operator+(Distance n) {
        return self(current - n);
    }
    self& operator+=(Distance n) {
        current -= n;
        return *this;
    }
    self operator-(Distance n) {
        return self(current + n);
    }
    self& operator-=(Distance n) {
        current += n;
        return *this;
    }
    reference operator[](Distance n) const { return *(*this); }
};

// 全局比较操作符
template<typename RandomAccessIterator,
         typename T,
         typename Reference,
         typename Distance>
inline bool operator<(reverse_iterator<RandomAccessIterator, T, Reference, Distance>& lhs,
                       reverse_iterator<RandomAccessIterator, T, Reference, Distance>& rhs) {
    return lhs.base() < rhs.base();
}

template<typename RandomAccessIterator,
         typename T,
         typename Reference,
         typename Distance>
inline bool operator<=(reverse_iterator<RandomAccessIterator, T, Reference, Distance>& lhs,
                       reverse_iterator<RandomAccessIterator, T, Reference, Distance>& rhs) {
    return lhs.base() <= rhs.base();
}

template<typename RandomAccessIterator,
         typename T,
         typename Reference,
         typename Distance>
inline bool operator>(reverse_iterator<RandomAccessIterator, T, Reference, Distance>& lhs,
                       reverse_iterator<RandomAccessIterator, T, Reference, Distance>& rhs) {
    return lhs.base() > rhs.base();
}

template<typename RandomAccessIterator,
         typename T,
         typename Reference,
         typename Distance>
inline bool operator>=(reverse_iterator<RandomAccessIterator, T, Reference, Distance>& lhs,
                       reverse_iterator<RandomAccessIterator, T, Reference, Distance>& rhs) {
    return lhs.base() >= rhs.base();
}

template<typename RandomAccessIterator,
         typename T,
         typename Reference,
         typename Distance>
inline bool operator==(reverse_iterator<RandomAccessIterator, T, Reference, Distance>& lhs,
                       reverse_iterator<RandomAccessIterator, T, Reference, Distance>& rhs) {
    return lhs.base() == rhs.base();
}

template<typename RandomAccessIterator,
         typename T,
         typename Reference,
         typename Distance>
inline bool operator!=(reverse_iterator<RandomAccessIterator, T, Reference, Distance>& lhs,
                       reverse_iterator<RandomAccessIterator, T, Reference, Distance>& rhs) {
    return lhs.base() != rhs.base();
}

template<typename RandomAccessIterator,
         typename T,
         typename Reference,
         typename Distance>
inline Distance operator-(reverse_iterator<RandomAccessIterator, T, Reference, Distance>& lhs,
                       reverse_iterator<RandomAccessIterator, T, Reference, Distance>& rhs) {
    return lhs.base() - rhs.base();
}

template<typename RandomAccessIterator,
         typename T,
         typename Reference,
         typename Distance>
inline reverse_iterator<RandomAccessIterator, T, Reference, Distance> operator+(Distance n,
        reverse_iterator<RandomAccessIterator, T, Reference, Distance>& iter) {
    return reverse_iterator<RandomAccessIterator, T, Reference, Distance>(iter.base() - n);
}

}
#endif /* INCLUDE_ITERATOR_H__ */