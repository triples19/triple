#include <cstdlib>
#include <glad/glad.h>

import triple.all;

int main() {
    using namespace triple;

    App app;
    app.add_plugin<WindowPlugin>()
        .add_system(
            StartUp,
            +[]() {
                log::info("Hello World!");
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
