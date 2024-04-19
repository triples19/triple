import triple.base;
import triple.math;
import triple.refl;
import triple.ecs;

struct Obj {
    int m_id;
    int m_buf[10000];
    int foo(int& x) {
        triple::log::info("foo {}", x);
        x = 10;
        return x * 2;
    }
};

namespace triple::refl {
template<>
const Type* type<Obj>() {
    static Type ty("Obj", sizeof(Obj));
    return &ty;
}
} // namespace triple::refl

int main() {
    using namespace triple;
    Obj obj;
    obj.m_id = 233;
    refl::Ref ref {obj};
    log::info("{} {}", ref.address(), ref.type()->name());
    refl::Value val {obj};
    log::info("{} {} {}", val.ref().address(), val.type()->name(), val.ref().value<Obj>().m_id);
    refl::Var var {val};
    log::info("{} {} {}", var.ref().address(), var.type()->name(), var.ref().value<Obj>().m_id);
    refl::Ref ref2 = val.ref();
    log::info("{} {} {}", ref2.address(), ref2.type()->name(), ref2.value<Obj>().m_id);

    auto m = refl::TMember("m_id", &Obj::m_id);
    log::info("{} {}", m.type()->name(), m.get(obj).value<int>());
    m.set(obj, 123);
    log::info("{} {}", m.type()->name(), m.get(obj).value<int>());

    auto method = refl::TMethod("foo", &Obj::foo, {"x"});
    int x = 1;
    refl::Var ret = method.invoke(obj, x);
    log::info("{} {}", ret.value<int>(), x);

    return 0;
}
