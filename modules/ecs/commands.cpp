module triple.ecs;

namespace triple::ecs {

Commands::Commands(World& world) : m_world(world) {}

EntityCommands Commands::spawn() {
    return {m_world, m_world.entity()};
}

EntityCommands Commands::entity(Entity id) {
    return {m_world, id};
}

EntityCommands::EntityCommands(World& world, Entity entity) : m_world(world), m_entity(entity) {}

EntityCommands& EntityCommands::add(refl::Ref component) {
    m_world.add_component(m_entity, component);
    return *this;
}

EntityCommands& EntityCommands::remove(const refl::Type& component_type) {
    m_world.remove_component(m_entity, component_type);
    return *this;
}

void EntityCommands::despawn() {
    m_world.despawn(m_entity);
}

Entity EntityCommands::id() {
    return m_entity;
}

} // namespace triple::ecs
