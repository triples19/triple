export module triple.base:primitive_types;

import std.compat;

namespace triple {

using i8  = int8_t;
using u8  = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using c32 = char32_t;

using byte = std::byte;

using hash16 = u16;
using hash32 = u32;
using hash64 = u64;

using id32 = u32;
using id64 = u64;

} // namespace triple
