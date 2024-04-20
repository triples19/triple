#include <cstdlib>

import triple.base;
import triple.math;
import triple.refl;
import triple.ecs;
import triple.window;
import triple.app;

int main() {
    using namespace triple;

    app::App app;

    window::Window win;
    win.setup(app);

    while (!win.should_close()) {
        win.poll_events();
        win.swap_buffers();
    }

    return 0;
}
