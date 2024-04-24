module triple.ecs;
import :commands;
import :world;

namespace triple::ecs {

Commands::Commands(World& world) : m_world(world) {}

}