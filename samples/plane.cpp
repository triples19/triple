#define NOMODULE
#include "refl_macros.hpp"

#include "imgui.h"

#include <cassert>
#include <filesystem>
#include <unordered_set>

import triple.all;
import samples.ui;
import samples.common;

using namespace triple;

constexpr float player_speed = 1000.0f;
constexpr float player_shoot_cooldown = 0.3f;
constexpr float player_bullet_speed = 1000.0f;
constexpr float enemy_speed_min = 300.0f;
constexpr float enemy_speed_max = 400.0f;
constexpr float enemy_spawn_cooldown = 0.5f;
constexpr float enemy_shoot_cooldown = 1.2f;
constexpr float enemy_bullet_speed = 500.0f;
constexpr int player_init_health = 3;
constexpr int enemy_init_health = 2;

float global_scale = 3.0f;
bool show_debug = false;

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
    Timer shoot_timer;
    float speed;
    int health;
};
REFL(Player)

struct Enemy {
    float speed;
    int health;
    Timer shoot_timer;
};
REFL(Enemy)

struct Bullet {
    Vector2 speed;
    enum Tag { Player, Enemy } tag;
};
REFL(Bullet)

struct Game {
    float enemy_spawn_timer {0.0f};
    int score {0};
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

void check_bullet_collide(
    Query<Bullet, BoxCollider, Transform2D> q_bullet,
    Query<Enemy, BoxCollider, Transform2D> q_enemy,
    Query<Player, BoxCollider, Transform2D> q_player,
    Commands commands
) {
    std::unordered_set<Entity> to_despawn;
    auto [player, collider_player, transform_player] = *q_player.begin();
    for (auto iter1 = q_bullet.begin(); iter1 != q_bullet.end(); ++iter1) {
        if (to_despawn.find(iter1.entity()) != to_despawn.end())
            continue;
        auto [bullet, collider_bullet, transform_bullet] = *iter1;
        auto rect_bullet = collider_bullet.rect_global(transform_bullet);
        auto rect_player = collider_player.rect_global(transform_player);
        if (bullet.tag == Bullet::Enemy &&
            rect_collide(rect_bullet, rect_player)) {
            to_despawn.insert(iter1.entity());
            player.health--;
            continue;
        }
        for (auto iter2 = q_enemy.begin(); iter2 != q_enemy.end(); ++iter2) {
            if (to_despawn.find(iter2.entity()) != to_despawn.end())
                continue;
            auto [enemy, collider_enemy, transform_enemy] = *iter2;
            if (bullet.tag == Bullet::Enemy)
                continue;
            auto rect_enemy = collider_enemy.rect_global(transform_enemy);
            if (rect_collide(rect_bullet, rect_enemy)) {
                to_despawn.insert(iter1.entity());
                enemy.health--;
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
            .health = player_init_health,
        },
        Sprite {
            .texture = asset_server->load<Texture2D>("ship_0.png"),
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
    if (!show_debug)
        return;
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
                .speed = {0.0f, player_bullet_speed},
                .tag = Bullet::Player,
            },
            Sprite {
                .texture = asset_server->load<Texture2D>("bullet_0.png"),
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

void update_bullet(
    Query<Bullet, Transform2D> q_bullet,
    Resource<Time> time,
    Resource<Window> win,
    Commands commands
) {
    for (auto iter = q_bullet.begin(); iter != q_bullet.end(); ++iter) {
        auto [bullet, transform] = *iter;
        auto entity = iter.entity();
        transform.position += bullet.speed * time->delta();
        if (transform.position.y > win->height / 2 + 8.0f * 3.0f ||
            transform.position.y < -win->height / 2 - 8.0f * 3.0f) {
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
            Enemy {
                .speed = (float)random(enemy_speed_min, enemy_speed_max),
                .health = enemy_init_health,
                .shoot_timer =
                    Timer(enemy_shoot_cooldown, TimerMode::Repeating),
            },
            Sprite {
                .texture = asset_server->load<Texture2D>("ship_1.png"),
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

void update_enemy(
    Commands commands,
    Resource<AssetServer> asset_server,
    Query<Enemy, Transform2D, BoxCollider> q_enemy,
    Query<Player, Transform2D, BoxCollider> q_player,
    Resource<Time> time,
    Resource<Window> win,
    Resource<Game> game
) {
    auto [player, transform_player, collider_player] = *q_player.begin();
    for (auto iter = q_enemy.begin(); iter != q_enemy.end(); ++iter) {
        auto [enemy, transform, collider] = *iter;
        if (enemy.health <= 0) {
            commands.entity(iter.entity()).despawn();
            game->score++;
            continue;
        }
        if (rect_collide(
                collider.rect_global(transform),
                collider_player.rect_global(transform_player)
            )) {
            commands.entity(iter.entity()).despawn();
            player.health--;
            game->score++;
            continue;
        }
        transform.position.y -= enemy.speed * time->delta();
        if (transform.position.y < -win->height / 2 - 16.0f * 3.0f) {
            commands.entity(iter.entity()).despawn();
        }
        enemy.shoot_timer.tick(time->delta());
        if (enemy.shoot_timer.just_finished()) {
            commands.spawn().add(
                Bullet {
                    .speed = {0.0f, -enemy_bullet_speed},
                    .tag = Bullet::Enemy,
                },
                Sprite {
                    .texture = asset_server->load<Texture2D>("bullet_1.png"),
                },
                Transform2D {
                    .position =
                        transform.position - Vector2 {0.0f, 16.0f * 3.0f},
                    .scale = {3.0f, 3.0f},
                },
                BoxCollider {
                    .size = {8.0f, 16.0f},
                    .offset = {0.0f, 0.0f},
                }
            );
        }
    }
}

void update_ui(
    Commands commands,
    Query<Player, Transform2D, Sprite, BoxCollider> q_player,
    Query<Bullet> q_bullet,
    Query<Enemy> q_enemy,
    Resource<Game> game,
    Resource<Time> time,
    Resource<AppStates> app_states
) {
    auto [player, transform, sprite, collider] = *q_player.begin();
    ImGui::Begin("Game");

    ImGui::Text("Score: %d", game->score);
    ImGui::Text("Health: %d", player.health);
    if (ImGui::Button("Show Debug")) {
        show_debug = !show_debug;
    }

    // if (ImGui::CollapsingHeader("Player")) {
    //     ImGui::InputInt("HP", &player.health, 0, 0);
    //     ImGui::InputFloat2("Position", &transform.position.x);
    // }

    static bool modal_open = false;
    if (player.health <= 0 && !modal_open) {
        ImGui::OpenPopup("Game Over");
        modal_open = true;
        time->time_scale = 0.0f;
    }

    if (ImGui::BeginPopupModal(
            "Game Over",
            NULL,
            ImGuiWindowFlags_AlwaysAutoResize
        )) {
        ImGui::Text("Game Over!");
        ImGui::Text("Score: %d", game->score);
        if (ImGui::Button("Restart")) {
            game->score = 0;
            player.health = player_init_health;
            ImGui::CloseCurrentPopup();
            modal_open = false;
            time->time_scale = 1.0f;

            for (auto iter = q_bullet.begin(); iter != q_bullet.end(); ++iter) {
                commands.entity(iter.entity()).despawn();
            }
            for (auto iter = q_enemy.begin(); iter != q_enemy.end(); ++iter) {
                commands.entity(iter.entity()).despawn();
            }
            transform.position = {0.0f, 0.0f};
        }
        ImGui::SameLine();
        if (ImGui::Button("Quit")) {
            ImGui::CloseCurrentPopup();
            modal_open = false;
            time->time_scale = 1.0f;
            app_states->should_stop = true;
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

int main() {
    register_types();
    App app;
    app.add_plugin<SamplesPlugin>()
        .add_plugin<UiPlugin>()
        .add_event<CollideEvent>()
        .add_resource<Game>()
        .add_system(StartUp, setup_scene)
        .add_system(Update, check_bullet_collide)
        .add_system(Update, draw_debug)
        .add_system(Update, move_player)
        .add_system(Update, player_shoot)
        .add_system(Update, update_bullet)
        .add_system(Update, spawn_enemy)
        .add_system(Update, update_enemy)
        .add_system(RenderUpdate, update_ui)
        .run();
}
