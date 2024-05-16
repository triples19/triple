#include <cstdlib>
#include <ctime>

#include "box2d/box2d.h"
#include "imgui.h"

#define NOMODULE
#include "refl_macros.hpp"

import triple.all;
import samples.common;
import samples.ui;

b2Vec2 gravity {0.0f, -200.0f};

struct Physics {
    b2World world {gravity};
};
REFL(Physics)
REFL(b2World)

struct RigidBody {
    b2Body* body {nullptr};
    bool initialized {false};
};
REFL(RigidBody)
REFL(b2Body)

void setup_scene(
    triple::Commands commands,
    triple::Resource<triple::AssetServer> asset_server,
    triple::Resource<triple::Window> win,
    triple::Resource<Physics> physics,
    triple::Resource<triple::RenderResource> render_res
) {
    using namespace triple;
    render_res->clear_color_value = {1.0f, 1.0f, 1.0f, 1.0f};
    commands.spawn()
        .add(Camera {
            .width = (float)win->width,
            .height = (float)win->height,
            .near = -1.0f,
            .far = 1.0f,
        })
        .add(Transform2D {
            .position = {0.0f, 0.0f},
        });

    b2BodyDef ground_body_def;
    ground_body_def.position.Set(0.0f, 0.0f);
    b2Body* ground_body = physics->world.CreateBody(&ground_body_def);
    b2ChainShape shape;
    b2Vec2 points[4] = {
        {win->width / 2.0f, win->height / 2.0f},
        {win->width / 2.0f, -win->height / 2.0f},
        {-win->width / 2.0f, -win->height / 2.0f},
        {-win->width / 2.0f, win->height / 2.0f},
    };
    shape.CreateLoop(points, 4);
    ground_body->CreateFixture(&shape, 0.0f);
}

void update_physics(
    triple::Query<triple::Transform2D, RigidBody> query,
    triple::Resource<triple::Debug> debug,
    triple::Resource<Physics> physics,
    triple::Resource<triple::Time> time
) {
    using namespace triple;
    for (auto iter = query.begin(); iter != query.end(); ++iter) {
        Transform2D& tf = iter.get<Transform2D>();
        RigidBody& rb = iter.get<RigidBody>();

        if (!rb.initialized) {
            rb.initialized = true;
            rb.body->SetTransform({tf.position.x, tf.position.y}, 0.0f);
        }
        tf.position = {rb.body->GetPosition().x, rb.body->GetPosition().y};

        b2Body* body = rb.body;
        b2Fixture* fixture = body->GetFixtureList();
        b2Shape* shape = fixture->GetShape();
        if (shape->GetType() == b2Shape::e_polygon) {
            b2PolygonShape* polygon = (b2PolygonShape*)shape;
            b2Vec2 vertices[8];
            for (int i = 0; i < polygon->m_count; i++) {
                vertices[i] = body->GetWorldPoint(polygon->m_vertices[i]);
            }
            for (int i = 0; i < polygon->m_count; i++) {
                debug->line(
                    {vertices[i].x, vertices[i].y},
                    {vertices[(i + 1) % polygon->m_count].x,
                     vertices[(i + 1) % polygon->m_count].y},
                    {1, 0, 0, 1}
                );
            }
        } else if (shape->GetType() == b2Shape::e_circle) {
            b2CircleShape* circle = (b2CircleShape*)shape;
            b2Vec2 center = body->GetWorldPoint(circle->m_p);
            debug->circle({center.x, center.y}, circle->m_radius, 64);
        } else if (shape->GetType() == b2Shape::e_chain) {
            b2ChainShape* chain = (b2ChainShape*)shape;
            for (int i = 0; i < chain->m_count - 1; i++) {
                b2Vec2 v1 = body->GetWorldPoint(chain->m_vertices[i]);
                b2Vec2 v2 = body->GetWorldPoint(chain->m_vertices[i + 1]);
                debug->line({v1.x, v1.y}, {v2.x, v2.y}, {0, 1, 0, 1});
            }
        }
    }

    physics->world.Step(time->delta(), 6, 2);
}

void gen_ball(
    triple::Commands commands,
    triple::Resource<triple::AssetServer> asset_server,
    triple::Resource<Physics> physics,
    triple::Resource<triple::Window> win,
    triple::Resource<triple::MouseInput> input
) {
    auto& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;
    }
    using namespace triple;
    if (input->just_pressed(MouseButton::Left)) {
        auto pos = input->position();
        pos.y = win->height - pos.y;
        pos.x -= win->width / 2.0f;
        pos.y -= win->height / 2.0f;
        log::info("{} {}", pos.x, pos.y);
        b2BodyDef body_def;
        body_def.type = b2_dynamicBody;
        body_def.position.Set(0.0f, 0.0f);
        b2Body* body = physics->world.CreateBody(&body_def);
        b2CircleShape dynamic_box;
        dynamic_box.m_radius = 64.0f;
        b2FixtureDef fixture_def;
        fixture_def.shape = &dynamic_box;
        fixture_def.density = 1.0f;
        fixture_def.friction = 0.3f;
        fixture_def.restitution = 2.0f;
        body->CreateFixture(&fixture_def);

        auto get_random = []() -> float {
            return (double)rand() / (RAND_MAX);
        };

        commands.spawn().add(
            Sprite {
                .color = {get_random(), get_random(), get_random(), 1.0f},
                .anchor = Vector2 {0.5f, 0.5f},
                .texture = asset_server->load<Texture2D>("circle.png"),
            },
            Transform2D {
                .position = pos,
                .scale = {0.25f, 0.25f},
            },
            RigidBody {
                .body = body,
            }
        );
    }
}

int main() {
    srand(time(nullptr));
    using namespace triple;
    Cls::new_cls<RigidBody>()
        .add_member("body", &RigidBody::body)
        .add_member("initialized", &RigidBody::initialized);

    App app;
    app.add_plugin<SamplesPlugin>()
        .add_plugin<UiPlugin>()
        .add_resource<Physics>(Physics {.world = b2World(gravity)})
        .add_system(StartUp, setup_scene)
        .add_system(Update, update_physics)
        .add_system(Update, gen_ball)
        .run();
    exit(0);
    return 0;
}
