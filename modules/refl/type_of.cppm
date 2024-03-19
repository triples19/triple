export module triple.refl:type_of;
import :type;

import std;

namespace triple::refl {

export template<class T>
const Type* type() {
    static Type ty("INVALID", 0);
    return &ty;
}

template<typename T>
struct remove_qualifiers : std::type_identity<T> {};
template<typename T>
struct remove_qualifiers<T const> : remove_qualifiers<T> {};
template<typename T>
struct remove_qualifiers<T volatile> : remove_qualifiers<T> {};
template<typename T>
struct remove_qualifiers<T const volatile> : remove_qualifiers<T> {};
template<typename T>
struct remove_qualifiers<T&> : remove_qualifiers<T> {};
template<typename T>
struct remove_qualifiers<T&&> : remove_qualifiers<T> {};
template<typename T>
struct remove_qualifiers<T*> : remove_qualifiers<T> {};

template<typename T>
using raw_type = typename remove_qualifiers<T>::type;

export template<typename T>
const Type* type_of(T&&) {
    return type<raw_type<T>>();
}

} // namespace triple::refl
