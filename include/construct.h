#ifndef INCLUDE_CONSTRUCT_H__
#define INCLUDE_CONSTRUCT_H__

#include <new>

#include "iterator.h"
#include "type_traits.h"

namespace tinystl
{
// 全局构造函数
template<typename T>
inline void construct(T* ptr) {
    new (ptr) T(); // placement new
}

// 在ptr指向的内存上构造T1对象，使用value初始化
template<typename T1, typename T2>
inline void construct(T1* ptr, const T2& value) {
    new (ptr) T1(value);
}

template<typename T1, typename T2>
inline void construct(T1* ptr, T2&& value) {
    new (ptr) T1(value);
}

// 全局析构函数
template<typename T>
inline void destroy(T* ptr) {
    ptr->~T(); // 调用析构函数
}

template<typename ForwardIteraor>
inline void destroy_aux(ForwardIteraor first, ForwardIteraor last, _true_type) { }

template<typename ForwardIteraor>
inline void destroy_aux(ForwardIteraor first, ForwardIteraor last, _false_type) {
    for (; first != last; ++first) {
        destroy(&*first); // 调用析构函数
    }
}

template<typename ForwardIteraor>
inline void destroy(ForwardIteraor first, ForwardIteraor last) {
    // TODO: 后续将使用自己的 iterator_traits 替换 std::iterator_traits
    // using value_type = typename tinystl::iterator_traits<ForwardIteraor>::value_type;
    using value_type = typename std::iterator_traits<ForwardIteraor>::value_type;
    using is_trivial_destructor = typename tinystl::type_traits<value_type>::has_trivial_destructor;
    destroy_aux(first, last, is_trivial_destructor());
}
}
#endif /* INCLUDE_CONSTRUCT_H__ */