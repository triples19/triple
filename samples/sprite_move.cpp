#include "common.hpp"

void setup_scene(
    Commands commands,
    Resource<AssetServer> asset_server,
    Resource<Window> res_win
) {
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
    commands.spawn()
        .add(Sprite {
            .color = {1.0f, 1.0f, 1.0f, 1.0f},
            .anchor = Vector2 {0.5f, 0.5f},
            .texture = asset_server->load<Texture2D>("awesomeface.png"),
            .program = asset_server->load<Program>("sprite.glsl")
        })
        .add(Transform2D {
            .position = {0.0f, 0.0f},
            .scale = {0.5f, 0.5f},
        });
}

void move_sprite(
    Resource<KeyInput> key_input,
    Resource<Time> time,
    Query<Sprite, Transform2D> query
) {
    auto iter = query.iter();
    while (iter) {
        auto& transform = iter.get<Transform2D>();
        float speed = 200.0f;
        if (key_input->pressed(KeyCode::Up)) {
            transform.position.y += speed * time->delta();
        }
        if (key_input->pressed(KeyCode::Down)) {
            transform.position.y -= speed * time->delta();
        }
        if (key_input->pressed(KeyCode::Left)) {
            transform.position.x -= speed * time->delta();
        }
        if (key_input->pressed(KeyCode::Right)) {
            transform.position.x += speed * time->delta();
        }
        iter.next();
    }
}

int main() {
    App app;
    app.add_plugin<SamplesPlugin>()
        .add_plugin<TimePlugin>()
        .add_plugin<WindowPlugin>()
        .add_plugin<InputPlugin>()
        .add_plugin<OpenGLPlugin>()
        .add_plugin<GraphicsPlugin>()
        .add_plugin<RenderPlugin>()
        .add_plugin<SpritePlugin>()
        .add_system(StartUp, setup_scene)
        .add_system(Update, move_sprite)
        .run();

    return 0;
}
