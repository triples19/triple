#include "common.hpp"

int main() {
    App app;
    app.add_plugin<SamplesPlugin>()
        .add_plugin<WindowPlugin>()
        .add_plugin<OpenGLPlugin>()
        .add_plugin<GraphicsPlugin>()
        .add_plugin<RenderPlugin>()
        .add_plugin<DebugPlugin>()
        .add_system(
            StartUp,
            +[](Commands commands, Resource<Window> res_win) {
                commands.spawn()
                    .add(Camera {
                        .width = (float)res_win->width,
                        .height = (float)res_win->height,
                        .near = -1.0f,
                        .far = 1.0f,
                    })
                    .add(Transform2D {
                        .position = {0.0f, 0.0f},
                    });
            }
        )
        .add_system(
            Update,
            +[](Resource<Debug> debug) {
                debug->line({-200, 0}, {200, 0}, {1, 0, 0, 1});
                debug->rect({{-300, -300}, {300, 300}}, {0, 1, 0, 1});
            }
        )
        .run();

    return 0;
}
