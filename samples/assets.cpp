#include <filesystem>
#include <string>

import triple.all;

struct Text {
    std::string text;
};

namespace triple::refl {
template<>
const Type& type<Text>() {
    static Type ty("Text", sizeof(Text));
    return ty;
}
} // namespace triple::refl

using namespace triple;

class TextLoader : public AssetLoader<Text> {
  public:
    void* load(const std::filesystem::path& path) override { return new Text {read_file_str(path)}; }
};

int main() {
    App app;
    app.add_plugin<AssetPlugin>();
    AssetServer& asset_server = app.get_resource<AssetServer>().get();
    auto assets_dir = std::filesystem::current_path().parent_path().parent_path() / "samples" / "assets";
    asset_server.set_assets_dir(assets_dir);
    asset_server.add_loader<Text, TextLoader>();
    auto handle = asset_server.load<Text>("text.txt");
    auto& text = asset_server.get<Text>(handle);
    log::info("File:\n{}", text.text);

    return 0;
}
