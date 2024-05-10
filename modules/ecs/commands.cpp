module triple.ecs;
import triple.refl;

namespace triple::ecs {

Commands::Commands(World& world) : m_world(world) {}

EntityCommands Commands::spawn() {
    return {m_world, m_world.entity()};
}

EntityCommands Commands::entity(Entity id) {
    return {m_world, id};
}

EntityCommands::EntityCommands(World& world, Entity entity) :
    m_world(world), m_entity(entity) {}

EntityCommands& EntityCommands::add(refl::Value component) {
    m_world.add_command([entity = m_entity, component](World& world) {
        world.add_component(entity, component.ref());
    });
    return *this;
}

EntityCommands& EntityCommands::remove(const refl::Type& component_type) {
    m_world.add_command([entity = m_entity, &component_type](World& world) {
        world.remove_component(entity, component_type);
    });
    return *this;
}

bool EntityCommands::has_component(const refl::Type& component_type) {
    return m_world.has_component(m_entity, component_type);
}

void EntityCommands::despawn() {
    m_world.add_command([entity = m_entity](World& world) {
        world.despawn(entity);
    });
}

Entity EntityCommands::id() {
    return m_entity;
}

} // namespace triple::ecs
