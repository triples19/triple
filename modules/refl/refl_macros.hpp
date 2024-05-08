#pragma once

#define TYPE_DECL(name)       \
    namespace triple::refl {  \
    export template<>         \
    const Type& type<name>(); \
    }

#define TYPE_DEF(name)                       \
    namespace triple::refl {                 \
    template<>                               \
    const Type& type<name>() {               \
        static Type ty(#name, sizeof(name)); \
        return ty;                           \
    }                                        \
    }

#define TYPE(name)                           \
    namespace triple::refl {                 \
    export template<>                        \
    const Type& type<name>() {               \
        static Type ty(#name, sizeof(name)); \
        return ty;                           \
    }                                        \
    }

#ifdef NOMODULE
#    define REFL(name)                           \
        namespace triple::refl {                 \
        template<>                               \
        const Type& type<name>() {               \
            static Type ty(#name, sizeof(name)); \
            return ty;                           \
        }                                        \
        }
#else
#    define REFL(name)                           \
        namespace triple::refl {                 \
        export template<>                        \
        const Type& type<name>() {               \
            static Type ty(#name, sizeof(name)); \
            return ty;                           \
        }                                        \
        }
#endif
