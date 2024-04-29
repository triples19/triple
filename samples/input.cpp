#include "common.hpp"

void print_input(ecs::Resource<KeyInput> input) {
    for (KeyCode key : c_key_codes) {
        const char* key_name = key_code_to_string(key);
        // if (input->pressed(key)) {
        //     log::info("Pressed: {}", key_name);
        // }
        if (input->just_pressed(key)) {
            log::info("Just pressed: {}", key_name);
        }
        if (input->just_released(key)) {
            log::info("Just released: {}", key_name);
        }
    }
}

int main() {
    App app;
    app.add_plugin<SamplesPlugin>()
        .add_plugin<WindowPlugin>()
        .add_plugin<InputPlugin>()
        .add_system(Update, print_input)
        .run();

    return 0;
}
