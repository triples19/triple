#define NOMODULE
#include "refl_macros.hpp"
#include <filesystem>
import triple.all;
using namespace triple;

struct BoxCollider {
    Vector2 size;
    Vector2 offset;
    inline Rect rect_global(const Transform2D& transform) const {
        auto center = transform.position;
        auto size_scaled = size * transform.scale;
        return Rect {
            .min = center + offset - size_scaled / 2.0f,
            .max = center + offset + size_scaled / 2.0f,
        };
    }
};
REFL(BoxCollider)

struct CollideEvent {
    Entity entity_a;
    Entity entity_b;
};
REFL(CollideEvent)

struct Player {
    float shoot_cooldown;
    float shoot_timer;
    float speed;
};
REFL(Player)

struct Enemy {
    float speed;
};
REFL(Enemy)

struct Bullet {
    float speed;
};
REFL(Bullet)

void check_collide(
    Query<BoxCollider, Transform2D> q,
    EventWriter<CollideEvent> event
) {
    for (auto iter1 = q.begin(); iter1 != q.end(); ++iter1) {
        for (auto iter2 = q.begin(); iter2 != q.end(); ++iter2) {
            if (iter1.entity() == iter2.entity()) {
                continue;
            }
            auto [collider_a, transform_a] = *iter1;
            auto [collider_b, transform_b] = *iter2;
            auto rect_a = collider_a.rect_global(transform_a);
            auto rect_b = collider_b.rect_global(transform_b);

            // check if two rectangles collide
            if (rect_a.min.x < rect_b.max.x && rect_a.max.x > rect_b.min.x &&
                rect_a.min.y < rect_b.max.y && rect_a.max.y > rect_b.min.y) {
                event.send(CollideEvent {
                    .entity_a = iter1.entity(),
                    .entity_b = iter2.entity(),
                });
            }
        }
    }
}

void setup_scene(
    Commands commands,
    Resource<AssetServer> asset_server,
    Resource<Window> res_win
) {
    commands.spawn().add(
        Camera {
            .width = (float)res_win->width,
            .height = (float)res_win->height,
            .near = -1.0f,
            .far = 1.0f,
        },
        Transform2D {
            .position = {0.0f, 0.0f},
        }
    );
    commands.spawn().add(
        Player {
            .shoot_cooldown = 0.5f,
            .shoot_timer = 0.0f,
            .speed = 200.0f,
        },
        Sprite {
            .texture = asset_server->load<Texture2D>("ship_0.png"),
            .program = asset_server->load<Program>("sprite.glsl")
        },
        Transform2D {
            .position = {0.0f, 0.0f},
            .scale = {3.0f, 3.0f},
        },
        BoxCollider {
            .size = {32.0f, 32.0f},
            .offset = {0.0f, 0.0f},
        }
    );
    commands.spawn().add(
        Enemy {},
        Sprite {
            .texture = asset_server->load<Texture2D>("ship_1.png"),
            .program = asset_server->load<Program>("sprite.glsl")
        },
        Transform2D {
            .position = {100.0f, 100.0f},
            .scale = {3.0f, 3.0f},
            .rotation = PI,
        },
        BoxCollider {
            .size = {32.0f, 32.0f},
            .offset = {0.0f, 0.0f},
        }
    );
}

void draw_debug(
    Query<BoxCollider, Transform2D> q_player,
    Resource<Debug> debug
) {
    for (auto [collider, transform] : q_player) {
        debug->rect(collider.rect_global(transform), Color4F::Red);
    }
}

void move_player(
    Query<Player, Transform2D> q_player,
    Resource<KeyInput> key_input,
    Resource<Time> time
) {
    for (auto iter = q_player.begin(); iter != q_player.end(); ++iter) {
        auto [player, transform] = *iter;
        float speed = player.speed;
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
    }
}

void player_shoot(
    Query<Player, Transform2D> q_player,
    Commands commands,
    Resource<AssetServer> asset_server,
    Resource<Time> time
) {
    auto [player, transform] = *q_player.begin();
    while (player.shoot_timer > player.shoot_cooldown) {
        commands.spawn().add(
            Bullet {
                .speed = 200.0f,
            },
            Sprite {
                .texture = asset_server->load<Texture2D>("bullet_0.png"),
                .program = asset_server->load<Program>("sprite.glsl")
            },
            Transform2D {
                .position = transform.position,
                .scale = {3.0f, 3.0f},
            },
            BoxCollider {
                .size = {8.0f, 16.0f},
                .offset = {0.0f, 0.0f},
            }
        );
        player.shoot_timer -= player.shoot_cooldown;
    }
    player.shoot_timer += time->delta();
}

void move_bullet(Query<Bullet, Transform2D> q_bullet, Resource<Time> time) {
    for (auto [bullet, transform] : q_bullet) {
        transform.position.y += bullet.speed * time->delta();
    }
}

int main() {
    App app;
    app.add_plugin<AssetPlugin>();
    AssetServer& asset_server = app.get_resource<AssetServer>().get();
    auto assets_dir =
        std::filesystem::current_path().parent_path().parent_path() / "game" /
        "assets";
    asset_server.set_assets_dir(assets_dir);
    app.add_plugin<TimePlugin>()
        .add_plugin<WindowPlugin>()
        .add_plugin<InputPlugin>()
        .add_plugin<OpenGLPlugin>()
        .add_plugin<GraphicsPlugin>()
        .add_plugin<RenderPlugin>()
        .add_plugin<SpritePlugin>()
        .add_plugin<DebugPlugin>()
        .add_event<CollideEvent>()
        .add_system(StartUp, setup_scene)
        .add_system(Update, check_collide)
        .add_system(Update, draw_debug)
        .add_system(Update, move_player)
        .add_system(Update, player_shoot)
        .add_system(Update, move_bullet)
        .run();
}
