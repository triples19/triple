module;
#include <string>

module triple.ecs;
import triple.refl;

namespace triple::ecs {

System::System(World& world, SystemId id) :
    m_world(world), m_id(id), m_commands(*this) {}

System& System::add_query(const std::string& name, ComponentVector types) {
    m_queries[name] = &m_world.query(types);
    return *this;
}

System& System::add_event_reader(const refl::Type& event_type) {
    m_event_readers.emplace(event_type.id(), m_world.get_events(event_type));
    return *this;
}

System& System::add_event_wrtier(const refl::Type& event_type) {
    m_event_writers.emplace(event_type.id(), m_world.get_events(event_type));
    return *this;
}

System& System::add_resource(const refl::Type& resource_type) {
    m_resources.emplace(
        resource_type.id(),
        m_world.get_resource(resource_type)
    );
    return *this;
}

SystemCommands::SystemCommands(System& system) : m_system(system) {}

GenericQuery& SystemCommands::query(const std::string& name) {
    return *m_system.m_queries.at(name);
}

GenericEventReader& SystemCommands::event_reader(const refl::Type& event_type) {
    return m_system.m_event_readers.at(event_type.id());
}

GenericEventWriter& SystemCommands::event_writer(const refl::Type& event_type) {
    return m_system.m_event_writers.at(event_type.id());
}

GenericResource& SystemCommands::resource(const refl::Type& resource_type) {
    return m_system.m_resources.at(resource_type.id());
}

Commands SystemCommands::commands() {
    return Commands {m_system.m_world};
}

} // namespace triple::ecs
