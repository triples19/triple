# Triple

Triple is a simple, modular, data-driven game engine written in **modern C++**. It is inspired a lot by [Bevy](https://github.com/bevyengine/bevy).

## Current Status

As a game engine developed by a single person, Triple provides a limit amount of features today as shown below:

* **Module**: Using C++20 Modules for better code structure and faster compile time
* **Data**: Component memory management based on Archetypes
* **Graphics**: Basic 2D sprite rendering
* **Input**: Keyboard and mouse input
* **Assets**: An extendable assets system
* **Reflection**: Run-time reflection module
* **API**: Easy-to-use APIs similar to Bevy's

## Example

Below is a minimal example of Triple's core ECS functionality.

```C++
import triple.all;
using namespace triple;

struct Position {
    float x;
    float y;
};

struct Velocity {
    float dx;
    float dy;
};

void start(Commands commands) {
    for (auto i = 0u; i < 10u; ++i) 
        commands.spawn()
            .add(Position {.x = i * 1.f, .y = i * 1.f})
            .add(Velocity {.dx = 0.1f, .dy = 0.1f});
}

void update(Query<Position, Velocity> query) {
    for (auto [pos, vel] : query) {
        pos.x += vel.dx;
        pos.y += vel.dy;
    }
}

int main() {
    App()
        .add_system(StartUp, start)
        .add_system(Update, update)
        .run();
    return 0;
}
```

## Build

First, follow the instructions to install [xmake](https://github.com/xmake-io/xmake).

Once xmake is ready, simply clone the repository and run xmake. Dependencies will be downloaded and installed automatically by xmake.

```
git clone https://github.com/triples19/triple.git
cd triple
xmake
```

After building is done, you can run the samples such as the plane shooting game `samples-plane`.

```
xmake run samples-plane
```

