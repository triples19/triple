#pragma once

#ifdef NDEBUG
#    define tr_assert(condition, message) static_cast<void>(0)
#else
#    define tr_assert(condition, message)        \
        do {                                     \
            if (!(condition))                    \
                triple::log::fatal(              \
                    "Assertion '{}' failed: {}", \
                    #condition,                  \
                    message                      \
                );                               \
        } while (0)
#endif
