#define NOMODULE
#include "refl_macros.hpp"
#include <cassert>
#include <filesystem>
#include <unordered_set>

import triple.all;
import samples.ui;
import samples.common;

using namespace triple;

constexpr float player_speed = 800.0f;
constexpr float player_shoot_cooldown = 0.5f;
constexpr float enemy_speed = 400.0f;
constexpr float bullet_speed = 1000.0f;
constexpr float enemy_spawn_cooldown = 1.0f;

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
    // float shoot_cooldown;
    // float shoot_timer;
    Timer shoot_timer;
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

struct Game {
    float enemy_spawn_timer {0.0f};
};
REFL(Game)

void register_types() {
    Cls::new_cls<BoxCollider>()
        .add_member("size", &BoxCollider::size)
        .add_member("offset", &BoxCollider::offset);
    Cls::new_cls<CollideEvent>()
        .add_member("entity_a", &CollideEvent::entity_a)
        .add_member("entity_b", &CollideEvent::entity_b);
    Cls::new_cls<Player>()
        .add_member("shoot_timer", &Player::shoot_timer)
        .add_member("speed", &Player::speed);
    Cls::new_cls<Enemy>().add_member("speed", &Enemy::speed);
    Cls::new_cls<Bullet>().add_member("speed", &Bullet::speed);
}

int random(int min, int max) {
    static bool first = true;
    if (first) {
        srand(time(NULL));
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}

bool rect_collide(const Rect& a, const Rect& b) {
    return a.min.x < b.max.x && a.max.x > b.min.x && a.min.y < b.max.y &&
           a.max.y > b.min.y;
}

void check_bullet_enemy_collide(
    Query<Bullet, BoxCollider, Transform2D> q_bullet,
    Query<Enemy, BoxCollider, Transform2D> q_enemy,
    Commands commands
) {
    std::unordered_set<Entity> to_despawn;
    for (auto iter1 = q_bullet.begin(); iter1 != q_bullet.end(); ++iter1) {
        if (to_despawn.find(iter1.entity()) != to_despawn.end()) {
            continue;
        }
        for (auto iter2 = q_enemy.begin(); iter2 != q_enemy.end(); ++iter2) {
            if (to_despawn.find(iter2.entity()) != to_despawn.end()) {
                continue;
            }
            auto [bullet, collider_bullet, transform_bullet] = *iter1;
            auto [enemy, collider_enemy, transform_enemy] = *iter2;
            auto rect_bullet = collider_bullet.rect_global(transform_bullet);
            auto rect_enemy = collider_enemy.rect_global(transform_enemy);
            if (rect_collide(rect_bullet, rect_enemy)) {
                to_despawn.insert(iter1.entity());
                to_despawn.insert(iter2.entity());
            }
        }
    }
    for (auto entity : to_despawn) {
        commands.entity(entity).despawn();
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
            .shoot_timer = Timer(player_shoot_cooldown, TimerMode::Repeating),
            .speed = player_speed,
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
        // log::info("{} {}", transform.position.x, transform.position.y);
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
    Resource<Time> time,
    Resource<KeyInput> key_input
) {
    auto [player, transform] = *q_player.begin();
    player.shoot_timer.tick(time->delta());
    if (player.shoot_timer.just_finished()) {
        commands.spawn().add(
            Bullet {
                .speed = bullet_speed,
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
    }
}

void move_bullet(
    Query<Bullet, Transform2D> q_bullet,
    Resource<Time> time,
    Resource<Window> win,
    Commands commands
) {
    for (auto iter = q_bullet.begin(); iter != q_bullet.end(); ++iter) {
        auto [bullet, transform] = *iter;
        auto entity = iter.entity();
        transform.position.y += bullet_speed * time->delta();
        if (transform.position.y > win->height / 2 + 8.0f * 3.0f) {
            commands.entity(entity).despawn();
        }
    }
}

void spawn_enemy(
    Commands commands,
    Resource<AssetServer> asset_server,
    Resource<Time> time,
    Resource<Game> game,
    Resource<Window> win
) {
    while (game->enemy_spawn_timer > enemy_spawn_cooldown) {
        commands.spawn().add(
            Enemy {},
            Sprite {
                .texture = asset_server->load<Texture2D>("ship_1.png"),
                .program = asset_server->load<Program>("sprite.glsl")
            },
            Transform2D {
                .position =
                    {(float)random(-win->width / 2, win->width / 2),
                     (float)win->height / 2 + 16.0f * 3.0f},
                .scale = {3.0f, 3.0f},
                .rotation = PI,
            },
            BoxCollider {
                .size = {32.0f, 32.0f},
                .offset = {0.0f, 0.0f},
            }
        );
        game->enemy_spawn_timer -= enemy_spawn_cooldown;
    }
    game->enemy_spawn_timer += time->delta();
}

void move_enemy(
    Commands commands,
    Query<Enemy, Transform2D> q_enemy,
    Resource<Time> time,
    Resource<Window> win
) {
    std::vector<Entity> to_despawn;
    for (auto iter = q_enemy.begin(); iter != q_enemy.end(); ++iter) {
        auto [enemy, transform] = *iter;
        transform.position.y -= enemy_speed * time->delta();
        if (transform.position.y < -win->height / 2 - 16.0f * 3.0f) {
            to_despawn.push_back(iter.entity());
        }
    }
    for (auto entity : to_despawn) {
        commands.entity(entity).despawn();
    }
}

void pause_system(Resource<KeyInput> key_input, Resource<Time> time) {
    if (key_input->just_pressed(KeyCode::P)) {
        if (time->time_scale == 0.0f) {
            time->time_scale = 1.0f;
        } else {
            time->time_scale = 0.0f;
        }
    }
}

int main() {
    register_types();
    App app;
    app.add_plugin<SamplesPlugin>()
        .add_plugin<TimePlugin>()
        .add_plugin<WindowPlugin>()
        .add_plugin<InputPlugin>()
        .add_plugin<OpenGLPlugin>()
        .add_plugin<GraphicsPlugin>()
        .add_plugin<RenderPlugin>()
        .add_plugin<SpritePlugin>()
        .add_plugin<DebugPlugin>()
        .add_plugin<UiPlugin>()
        .add_event<CollideEvent>()
        .add_resource<Game>()
        .add_system(StartUp, setup_scene)
        .add_system(Update, check_bullet_enemy_collide)
        .add_system(Update, draw_debug)
        .add_system(Update, move_player)
        .add_system(Update, player_shoot)
        .add_system(Update, move_bullet)
        .add_system(Update, spawn_enemy)
        .add_system(Update, move_enemy)
        .add_system(Update, pause_system)
        .run();
}
