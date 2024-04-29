#pragma once
#include <filesystem>
import triple.all;

using namespace triple;

class SamplesPlugin : public Plugin {
  public:
    void setup(App& app) {
        app.add_plugin<AssetPlugin>();
        AssetServer& asset_server = app.get_resource<AssetServer>().get();
        auto assets_dir =
            std::filesystem::current_path().parent_path().parent_path() /
            "samples" / "assets";
        asset_server.set_assets_dir(assets_dir);
    }
};
