#ifndef INCLUDE_ALLOCATOR_H__
#define INCLUDE_ALLOCATOR_H__

#include "construct.h"

namespace tinystl
{
template<typename T>
class allocator{
public:
    // 类型别名定义
    using value_type        = T;
    using pointer           = T*;
    using const_pointer     = const T*;
    using reference         = T&;
    using const_reference   = const T&;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;

public:
    // 类接口, 使用静态类型可以减少开销
    // 分配内存
    static T*               allocate();
    static T*               allocate(size_type n);
    // 释放内存
    static void             deallocate(T* ptr);
    static void             deallocate(T* ptr, size_type n);
    // 构造对象
    static void             construct(T* ptr);
    static void             construct(T* ptr, const T& value);
    static void             construct(T* ptr, T&& value);
    // 析构对象
    static void             destroy(T* ptr);
    static void             destroy(T* first, T* last);
    // 获取对象地址
    static T*               address(T& val);
    // 获取可分配 T 类型对象的最大数目
    // static size_t           max_size();  // TODO: 后续将实现
    // 使用 T 类型的 allocator 为 U 类型分配内存
    template<typename U>
    struct rebind {
        using other = allocator<U>;
    };
};

// 使用 ::operator new 分配内存并返回空指针
template<typename T>
T* allocator<T>::allocate() {
    return static_cast<T*>(::operator new(sizeof(T)));
}

template<typename T>
T* allocator<T>::allocate(size_type n) {
    return static_cast<T*>(::operator new(n * sizeof(T)));
}

// 使用 ::operator delete 释放内存
template<typename T>
void allocator<T>::deallocate(T* ptr) {
    if (ptr != nullptr) ::operator delete(ptr);
}

template<typename T>
void allocator<T>::deallocate(T* ptr, size_type n) {
    if (n == 0) return;
    if (ptr != nullptr) ::operator delete(ptr);
}

// 构造对象
template<typename T>
void allocator<T>::construct(T* ptr) {
    tinystl::construct(ptr);
}

template<typename T>
void allocator<T>::construct(T* ptr, const T& value) {
    tinystl::construct(ptr, value);
}

template<typename T>
void allocator<T>::construct(T* ptr, T&& value) {
    tinystl::construct(ptr, std::move(value));
}

// 析构对象
template<typename T>
void allocator<T>::destroy(T* ptr) {
    tinystl::destroy(ptr);
}

template<typename T>
void allocator<T>::destroy(T* first, T* last) {
    tinystl::destroy(first, last);
}

template<typename T>
T* allocator<T>::address(T& val) {
    return (T*)(&val);
}

// template<typename T>
// size_t allocator<T>::max_size() {
//     return size_t(WINT_MAX / sizeof(T));
// }

}
#endif /* INCLUDE_ALLOCATOR_H__ */