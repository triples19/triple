import triple.all;

using namespace triple;

struct Position {
    float x;
    float y;
};

void print_info(Ref ref) {
    const Type& type = ref.type();
    log::info("Name: {}", type.name());
    log::info("Size: {}", type.size());
    log::info("ID:   {}", type.id());
    auto& c = cls(type);
    for (auto [name, member] : c.members()) {
        log::info("Member: {}", member->name());
        log::info("\tType: {}", member->type().name());
        log::info("\tValue: {}", member->get(ref).value<float>());
    }
}

int main() {
    Cls::new_cls<Position>()
        .add_member("x", &Position::x)
        .add_member("y", &Position::y);
    Position pos {.x = 1, .y = 2};
    print_info(pos);
    return 0;
}
