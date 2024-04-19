export module triple.refl:builtin_types;
import :type_of;

#define T(name)                              \
    template<>                               \
    const Type* type<name>() {               \
        static Type ty(#name, sizeof(name)); \
        return &ty;                          \
    }

export namespace triple::refl {

template<>
const Type* type<void>() {
    static Type ty("void", 0);
    return &ty;
}

T(signed char);
T(unsigned char);
T(short int);
T(unsigned short int);
T(int);
T(long int);
T(unsigned long int);
T(long long int);
T(unsigned long long int);

T(bool);

T(float);
T(double);
T(long double);

} // namespace triple::refl
