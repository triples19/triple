export module triple.base:platform;

#define TRIPLE_PLATFORM_WINDOWS 1
#define TRIPLE_PLATFORM_LINUX 2
#define TRIPLE_PLATFORM_APPLE 3

#define TRIPLE_COMPILER_MSVC 1
#define TRIPLE_COMPILER_GNUC 2
#define TRIPLE_COMPILER_CLANG 3
#define TRIPLE_COMPILER_APPLECLANG 4

#define TRIPLE_ENDIAN_LITTLE 1
#define TRIPLE_ENDIAN_BIG 2

#define TRIPLE_ARCH_32 1
#define TRIPLE_ARCH_64 2

// Platform
// ref: https://github.com/cpredef/predef/blob/master/OperatingSystems.md
#if defined(__WIN32__) || defined(_WIN32)
#    define TRIPLE_PLATFORM TRIPLE_PLATFORM_WINDOWS
#elif defined(__APPLE__)
#    define TRIPLE_PLATFORM TRIPLE_PLATFORM_APPLE
#else
#    define TRIPLE_PLATFORM TRIPLE_PLATFORM_LINUX
#endif

// Compiler
// ref: https://github.com/cpredef/predef/blob/master/Compilers.md
#if defined(__clang__)
#    if defined(__apple_build_version__)
#        define TRIPLE_COMPILER TRIPLE_COMPILER_APPLECLANG
#    else
#        define TRIPLE_COMPILER TRIPLE_COMPILER_CLANG
#    endif
#    define TRIPLE_COMPILER_VERSION (((__clang_major__) * 100) + (__clang_minor__ * 10) + __clang_patchlevel__)
#elif defined(__GNUC__)
#    define TRIPLE_COMPILER TRIPLE_COMPILER_GNUC
#    define TRIPLE_COMPILER_VERSION (((__GNUC__) * 100) + (__GNUC_MINOR__ * 10) + __GNUC_PATCHLEVEL__)
#elif defined(_MSC_VER)
#    define TRIPLE_COMPILER TRIPLE_COMPILER_MSVC
#    define TRIPLE_COMPILER_VERSION _MSC_VER
#else
#    error "Unknown compiler"
#endif

// Architecture
// ref: https://github.com/cpredef/predef/blob/master/Architectures.md
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || \
    defined(__s390__) || defined(__s390x__) || defined(_LP64) || defined(__LP64__)
#    define TRIPLE_ARCH_TYPE TRIPLE_ARCH_64
#else
#    define TRIPLE_ARCH_TYPE TRIPLE_ARCH_32
#endif

// inline
#if TRIPLE_COMPILER == TRIPLE_COMPILER_MSVC
#    define TRIPLE_INLINE inline
#    define TRIPLE_FORCE_INLINE __forceinline
#elif TRIPLE_COMPILER == TRIPLE_COMPILER_GNUC
#    define TRIPLE_INLINE inline
#    define TRIPLE_FORCE_INLINE inline __attribute__((always_inline))
#elif TRIPLE_COMPILER == TRIPLE_COMPILER_CLANG || TRIPLE_COMPILER == TRIPLE_COMPILER_APPLECLANG
#    define TRIPLE_INLINE inline
#    define TRIPLE_FORCE_INLINE inline __attribute__((always_inline))
#else
#    define TRIPLE_INLINE inline
#    define TRIPLE_FORCE_INLINE inline // no force inline for other platforms possible
#endif

// warnings
#if TRIPLE_COMPILER == TRIPLE_COMPILER_GNUC
#    define TRIPLE_BEGIN_DISABLE_DEPRECATED_WARNING \
        _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#    define TRIPLE_END_DISABLE_DEPRECATED_WARNING _Pragma("GCC diagnostic pop")

#    define TRIPLE_BEGIN_DISABLE_CONDITIONAL_EXPR_WARNING
#    define TRIPLE_END_DISABLE_CONDITIONAL_EXPR_WARNING
#    if TRIPLE_COMP_VER >= 700
#        define TRIPLE_BEGIN_DISABLE_EXCEPT_TYPE_WARNING \
            _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnoexcept-type\"")
#        define TRIPLE_END_DISABLE_EXCEPT_TYPE_WARNING _Pragma("GCC diagnostic pop")
#    else
#        define TRIPLE_BEGIN_DISABLE_EXCEPT_TYPE_WARNING
#        define TRIPLE_END_DISABLE_EXCEPT_TYPE_WARNING
#    endif

#    if TRIPLE_COMP_VER >= 510
#        define TRIPLE_BEGIN_DISABLE_OVERRIDE_WARNING \
            _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wsuggest-override\"")
#        define TRIPLE_END_DISABLE_OVERRIDE_WARNING _Pragma("GCC diagnostic pop")
#    else
#        define TRIPLE_BEGIN_DISABLE_OVERRIDE_WARNING
#        define TRIPLE_END_DISABLE_OVERRIDE_WARNING
#    endif

#    if TRIPLE_COMP_VER >= 900
#        define TRIPLE_BEGIN_DISABLE_INIT_LIST_WARNING \
            _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Winit-list-lifetime\"")
#        define TRIPLE_END_DISABLE_INIT_LIST_WARNING _Pragma("GCC diagnostic pop")
#    else
#        define TRIPLE_BEGIN_DISABLE_INIT_LIST_WARNING
#        define TRIPLE_END_DISABLE_INIT_LIST_WARNING
#    endif

#    define TRIPLE_DECLARE_PLUGIN_CTOR __attribute__((constructor))
#    define TRIPLE_DECLARE_PLUGIN_DTOR __attribute__((destructor))
#elif TRIPLE_COMPILER == TRIPLE_COMPILER_CLANG || TRIPLE_COMPILER == TRIPLE_COMPILER_APPLECLANG

#    if defined(__has_warning) && __has_warning("-Wdeprecated-declarations")
#        define TRIPLE_BEGIN_DISABLE_DEPRECATED_WARNING \
            _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#        define TRIPLE_END_DISABLE_DEPRECATED_WARNING _Pragma("clang diagnostic pop")
#    else
#        define TRIPLE_BEGIN_DISABLE_DEPRECATED_WARNING
#        define TRIPLE_END_DISABLE_DEPRECATED_WARNING
#    endif

#    define TRIPLE_BEGIN_DISABLE_CONDITIONAL_EXPR_WARNING
#    define TRIPLE_END_DISABLE_CONDITIONAL_EXPR_WARNING

#    if defined(__has_warning) && __has_warning("-Wnoexcept-type")
#        define TRIPLE_BEGIN_DISABLE_EXCEPT_TYPE_WARNING \
            _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wnoexcept-type\"")
#        define TRIPLE_END_DISABLE_EXCEPT_TYPE_WARNING _Pragma("clang diagnostic pop")
#    else
#        define TRIPLE_BEGIN_DISABLE_EXCEPT_TYPE_WARNING
#        define TRIPLE_END_DISABLE_EXCEPT_TYPE_WARNING
#    endif

#    if defined(__has_warning) && __has_warning("-Winconsistent-missing-override")
#        define TRIPLE_BEGIN_DISABLE_OVERRIDE_WARNING \
            _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Winconsistent-missing-override\"")
#        define TRIPLE_END_DISABLE_OVERRIDE_WARNING _Pragma("clang diagnostic pop")
#    else
#        define TRIPLE_BEGIN_DISABLE_OVERRIDE_WARNING
#        define TRIPLE_END_DISABLE_OVERRIDE_WARNING
#    endif

#    define TRIPLE_BEGIN_DISABLE_INIT_LIST_WARNING
#    define TRIPLE_END_DISABLE_INIT_LIST_WARNING

#    define TRIPLE_DECLARE_PLUGIN_CTOR __attribute__((__constructor__))
#    define TRIPLE_DECLARE_PLUGIN_DTOR __attribute__((__destructor__))

#elif TRIPLE_COMPILER == TRIPLE_COMPILER_MSVC
#    define TRIPLE_BEGIN_DISABLE_DEPRECATED_WARNING __pragma(warning(push)) __pragma(warning(disable : 4996))
#    define TRIPLE_END_DISABLE_DEPRECATED_WARNING __pragma(warning(pop))

#    define TRIPLE_BEGIN_DISABLE_CONDITIONAL_EXPR_WARNING __pragma(warning(push)) __pragma(warning(disable : 4127))
#    define TRIPLE_END_DISABLE_CONDITIONAL_EXPR_WARNING __pragma(warning(pop))

#    define TRIPLE_BEGIN_DISABLE_EXCEPT_TYPE_WARNING
#    define TRIPLE_END_DISABLE_EXCEPT_TYPE_WARNING
#    define TRIPLE_DECLARE_PLUGIN_CTOR
#    define TRIPLE_DECLARE_PLUGIN_DTOR
#    define TRIPLE_BEGIN_DISABLE_OVERRIDE_WARNING
#    define TRIPLE_END_DISABLE_OVERRIDE_WARNING
#    define TRIPLE_BEGIN_DISABLE_INIT_LIST_WARNING
#    define TRIPLE_END_DISABLE_INIT_LIST_WARNING
#endif

namespace triple {

enum class Platform : unsigned int {
    Windows,
    Linux,
    Apple,
};
constexpr Platform c_platform = static_cast<Platform>(TRIPLE_PLATFORM);

enum class Compiler : unsigned int {
    MSVC,
    GNUC,
    Clang,
    AppleClang,
};
constexpr Compiler c_compiler = static_cast<Compiler>(TRIPLE_COMPILER);

enum class Arch : unsigned int {
    Bit32,
    Bit64,
};
constexpr Arch c_arch = static_cast<Arch>(TRIPLE_ARCH_TYPE);

} // namespace triple
