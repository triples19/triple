#include <cstdlib>
#include <glad/glad.h>

import triple.all;

struct Obj {
    int x;
};

int main() {
    using namespace triple;

    App app;
    app.add_plugin<WindowPlugin>()
        .add_system(
            StartUp,
            +[]() {
                log::info("Hello World!");
                log::info("{}", type<Obj>().name());
                log::info("{}", type<Obj>().size());
            }
        )
        .add_system(
            Update,
            +[]() {
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
        )
        .run();

    return 0;
}
