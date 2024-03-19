export module triple.refl:type;

import std;
import std.compat;

namespace triple::refl {

export constexpr unsigned int c_max_types = 1000U;

static uint32_t s_type_index = 2;

export class Type {
  public:
    explicit Type() : m_id(0), m_name("") {}
    explicit Type(const char* name, std::size_t size) : m_id(s_type_index++), m_name(name), m_size(size) {}
    explicit Type(const char* name, const Type* base, std::size_t size) : Type(name, size) {
        m_base = base;
    }
    ~Type() {}

    Type(Type&)                  = delete;
    Type& operator=(const Type&) = delete;
    Type(Type&&)                 = delete;

    bool is(const Type* type) const {
        if (type == this)
            return true;
        else if (m_base)
            return m_base->is(type);
        else
            return false;
    }

    // template<class T>
    // bool is() const {
    //     return this->is(type<T>());
    // }

    static const Type* type() {
        static Type ty(0);
        return &ty;
    }

    const char* name() const {
        return m_name;
    }

    std::size_t size() const {
        return m_size;
    }

  private:
    Type(int) : m_id(1), m_name("Type") {}

  private:
    std::uint32_t m_id;
    const char*   m_name;
    std::size_t   m_size;
    const Type*   m_base = nullptr;
};

} // namespace triple::refl
