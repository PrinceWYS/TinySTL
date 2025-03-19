#ifndef INCLUDE_TYPE_TRAITS_H__
#define INCLUDE_TYPE_TRAITS_H__

namespace tinystl
{
// 不包含任何成员的类型，用于表示 true 和 false
struct _true_type  { };
struct _false_type { };

// 模板类
template<typename type>
struct type_traits
{
    using has_trivial_default_constructor   = _false_type;
    using has_trivial_copy_constructor      = _false_type;
    using has_trivial_assignment_operator   = _false_type;
    using has_trivial_destructor            = _false_type;
    using is_POD_type                       = _false_type;
};

// 特化基本数据类型的 traits
template<>
struct type_traits<bool>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<char>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<unsigned char>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<signed char>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<wchar_t>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<short>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<unsigned short>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<int>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<unsigned int>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<long>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<unsigned long>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<long long>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<unsigned long long>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<float>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<double>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<long double>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

// 指针的偏特化版本
template<typename type>
struct type_traits<type*>
{
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructor      = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;

};

// 辅助类 辅助实现 true_type 与 false_type
template<typename T, T v>
struct iteral_constant
{
    using value_type    = T;
    using type          = iteral_constant<T, v>;

    static constexpr T value = v;
};

using true_type  = iteral_constant<bool, true>;
using false_type = iteral_constant<bool, false>;

// 接受普通类型时继承 false_type, 静态成员 value 为 false
template<typename T>
struct is_const: public false_type { };

// 偏特化使得接受 const 类型时继承 true_type, 静态成员 value 为 true
template<typename T>
struct is_const<const T>: public true_type { };

}

#endif /* INCLUDE_TYPE_TRAITS_H__ */