module triple.graphics;
import :program;
import :device;

namespace triple::graphics {

bool Program::load(const std::filesystem::path& path) {
    auto device = RenderDevice::instance();
    auto src = read_file_str(path);

    auto vertex_shader = device->create_shader();
    if (!vertex_shader->compile(ShaderStage::Vertex, {"#version 330 core\n", "#define VERTEX\n", src})) {
        return false;
    }

    auto fragment_shader = device->create_shader();
    if (!fragment_shader->compile(ShaderStage::Fragment, {"#version 330 core\n", "#define FRAGMENT\n", src})) {
        return false;
    }

    return link(*vertex_shader, *fragment_shader);
}

} // namespace triple::graphics
