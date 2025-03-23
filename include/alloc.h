#ifndef INCLUDE_ALLOC_H__
#define INCLUDE_ALLOC_H__

#include <stdlib.h>
#include <cstring>
#include <iostream>

#include "construct.h"

namespace tinystl
{
// mallo 和 free 实现的一级分配器
class malloc_alloc {
private:
    // 函数指针
    using FuncPtr = void(*)();

public:
    static void* allocate(size_t);
    static void deallocate(void* ptr) { free(ptr); }
    static void* reallocate(void*, size_t, size_t new_size);
    static FuncPtr set_malloc_handler(FuncPtr f);

private:
    // 处理 OOM
    static void* oom_malloc(size_t);
    static void* oom_realloc(void*, size_t);
    // 函数指针类型的静态成员
    // static void (*malloc_alloc_oom_handler)();
    static FuncPtr malloc_alloc_oom_handler;
};

void* malloc_alloc::allocate(size_t n) {
    void* result = malloc(n);
    if (result == 0) {
        result = malloc_alloc::oom_malloc(n);
    }
    return result;
}

void* malloc_alloc::reallocate(void* ptr, size_t old_size, size_t new_size) {
    void* result = realloc(ptr, new_size);
    if (result == 0) {
        result = malloc_alloc::oom_realloc(ptr, new_size);
    }
    return result;
}

typename malloc_alloc::FuncPtr malloc_alloc::set_malloc_handler(FuncPtr fptr) {
    FuncPtr old = malloc_alloc_oom_handler;
    malloc_alloc_oom_handler = fptr;
    return old;
}

void* malloc_alloc::oom_malloc(size_t n) {
    // 未设定 handler 信息, 报错并退出
    if (malloc_alloc_oom_handler == 0) {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }

    for (;;) {
        // 不断尝试调用 handler 函数, 直到分配成功
        malloc_alloc_oom_handler();
        void* result = malloc(n);
        if (result) {
            return result;
        }
    }
}

void* malloc_alloc::oom_realloc(void* ptr, size_t new_size) {
    if (malloc_alloc_oom_handler == 0) {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }

    for (;;) {
        malloc_alloc_oom_handler();
        void* result = realloc(ptr, new_size);
        if (result) {
            return result;
        }
    }
}

malloc_alloc::FuncPtr malloc_alloc::malloc_alloc_oom_handler = 0;

// 第二级分配器
class default_alloc {
private:
    enum { ALIGN = 8 };
    enum { MAX_BYTES = 128 };
    enum { NFRESSLISTS = MAX_BYTES / ALIGN };

    union obj {
        union obj* next;        // 维护链表指针
        char client_data[1];    // 数据区
    };

    static char* start_free;    // 内存池的头
    static char* end_free;      // 内存池的尾
    static size_t heap_size;
    static obj* volatile free_list[NFRESSLISTS]; // 空闲链表

private:
    // 将传入的参数改为 8 的倍数
    static size_t round_up(size_t bytes) {
        return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
    }

    // 获取空闲链表的索引
    static size_t freelist_index(size_t bytes) {
        return ((bytes + ALIGN - 1) / ALIGN - 1);
    }

    static void* refill(size_t n); // 填充空闲链表
    static char* chunk_alloc(size_t size, int& nobjs); // 分配内存块

public:
    static void* allocate(size_t n);
    static void deallocate(void* ptr, size_t n);
    static void* reallocate(void* ptr, size_t old_size, size_t new_size);
};

// 静态成员初始化
char* default_alloc::start_free = 0;
char* default_alloc::end_free = 0;
size_t default_alloc::heap_size = 0;
default_alloc::obj* volatile default_alloc::free_list[NFRESSLISTS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void* default_alloc::refill(size_t n) {
    // 设定请求节点个数
    int nobjs = 20;
    // chunck_alloc 从内存池中分配 传引用
    char* chunk = chunk_alloc(n, nobjs);
    if (nobjs == 1) { // 如果只分配了一个节点, 直接返回
        return static_cast<void*>(chunk);
    }

    // 否则, 将剩余的节点加入空闲链表
    void* result = static_cast<void*>(chunk);
    obj* next_obj, *cur_obj;
    obj* volatile* my_free_list = free_list + freelist_index(n);
    *my_free_list = next_obj = (obj*)(chunk + n);

    for (int i = 1; ; ++i) {
        cur_obj = next_obj;
        next_obj = (obj*)((char*)next_obj + n);
        if (nobjs - 1 == i) {
            cur_obj->next = 0; // 最后一个节点指向空
            break;
        }
        else {
            cur_obj->next = next_obj;
        }
    }

    return result;
}

char* default_alloc::chunk_alloc(size_t size, int& nobjs) {
    char* result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;

    // 若能满足需求 直接划拨
    if (bytes_left >= total_bytes) {
        result = start_free;
        start_free += total_bytes;
        return result;
    }
    // 若能提供至少一个节点 尽可能划拨
    else if (bytes_left >= size) {
        nobjs = bytes_left / size;
        result = start_free;
        start_free += size * nobjs;
        return result;
    }
    // 使用 malloc 请求 (当前需求 * 2 + 历史需求/16)
    else {
        size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);
        // 检查目前的内存能不能分配给更小的链表
        if (bytes_left > 0) {
            obj* volatile* my_free_list = free_list + freelist_index(bytes_left);
            ((obj*)start_free)->next = *my_free_list;   // 将剩余内存加入空闲链表
            *my_free_list = (obj*)start_free;           // 更新链表头
        }
        start_free = (char*)malloc(bytes_to_get); // 申请新的内存块
        // 检查是否申请成功
        if (start_free == 0) {  // 申请失败
            obj* volatile* my_free_list;
            obj* ptr = *my_free_list;
            for (size_t i = size; i <= MAX_BYTES; i += ALIGN) {
                my_free_list = free_list + freelist_index(i);
                ptr = *my_free_list;
                if (ptr != 0) {
                    *my_free_list = ptr->next;
                    start_free = (char*)ptr;
                    end_free = start_free + i;
                    return chunk_alloc(size, nobjs);
                }
            }
            end_free = 0;
            start_free = (char*)malloc_alloc::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs);
    }
}

void* default_alloc::allocate(size_t n) {
    // 大于128就调用第一级分配器
    if (n > MAX_BYTES) {
        return malloc_alloc::allocate(n);
    }
    // 否则去寻找对应链表
    obj* volatile* my_free_list = free_list + freelist_index(n);
    obj* result = *my_free_list;

    if (result == 0) {  // 链表为空, 调用 refill 填充链表
        return refill(n);
    }
    *my_free_list = (*my_free_list)->next; // 更新链表头
    return static_cast<void*>(result);
}

void default_alloc::deallocate(void* ptr, size_t n) {
    // 大于128就调用第一级分配器
    if (n > MAX_BYTES) {
        malloc_alloc::deallocate(ptr);
        return;
    }
    obj* p = static_cast<obj*>(ptr);
    obj* volatile* my_free_list = free_list + freelist_index(n);
    p->next = *my_free_list;
    *my_free_list = p;
}

void* default_alloc::reallocate(void* ptr, size_t old_size, size_t new_size) {
    // 新旧 size > 128
    if (old_size > MAX_BYTES && new_size > MAX_BYTES) {
        return realloc(ptr, new_size);
    }
    // 同一个区间不用操作
    if (round_up(old_size) == round_up(new_size)) {
        return ptr;
    }
    // 开辟新内存 复制内容 释放原内存
    void* result = allocate(new_size);
    size_t copy_size = new_size > old_size ? new_size : old_size;
    memcpy(result, ptr, copy_size);
    deallocate(ptr, old_size);
    return result;
}


// 模板类
template<typename T>
class alloc {
public:
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
    static size_t           max_size();
    // 使用 T 类型的 allocator 为 U 类型分配内存
    template<typename U>
    struct rebind {
        using other = alloc<U>;
    };
};

template<typename T>
T* alloc<T>::allocate() {
    return static_cast<T*>(default_alloc::allocate(sizeof(T)));
}

template<typename T>
T* alloc<T>::allocate(size_type n) {
    return n == 0 ? 0 : static_cast<T*>(default_alloc::allocate(n * sizeof(T)));
}

template<typename T>
void alloc<T>::deallocate(T* ptr) {
    if (ptr) {
        default_alloc::deallocate((void*)ptr, sizeof(T));
    }
}

template<typename T>
void alloc<T>::deallocate(T* ptr, size_type n) {
    if (n != 0) {
        default_alloc::deallocate((void*)ptr, n * sizeof(T));
    }
}

// 构造对象
template<typename T>
void alloc<T>::construct(T* ptr) {
    tinystl::construct(ptr);
}

template<typename T>
void alloc<T>::construct(T* ptr, const T& value) {
    tinystl::construct(ptr, value);
}

template<typename T>
void alloc<T>::construct(T* ptr, T&& value) {
    tinystl::construct(ptr, std::move(value));
}

// 析构对象
template<typename T>
void alloc<T>::destroy(T* ptr) {
    tinystl::destroy(ptr);
}

template<typename T>
void alloc<T>::destroy(T* first, T* last) {
    tinystl::destroy(first, last);
}

template<typename T>
T* alloc<T>::address(T& val) {
    return (T*)(&val);
}

template<typename T>
size_t alloc<T>::max_size() {
    return size_t(WINT_MAX / sizeof(T));
}
}
#endif /* INCLUDE_ALLOC_H__ */