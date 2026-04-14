#include "ResourceRegistry.h"

void ResourceRegistry::registerResource(ResourceDefinition t_definition)
{
	if (isRegistered(t_definition.m_id))
	{
		std::cout << "ResourceRegistry: Warning - ID: " << t_definition.m_id << " already registered, Ignoring.\n\n";
		return;
	}

	m_resources.emplace(t_definition.m_id, t_definition);

	std::cout << "ResourceRegistry: Resource [" << t_definition.m_id << "] "
		<< t_definition.m_name << "\n\n";
}

const ResourceDefinition* ResourceRegistry::getResource(uint32_t t_id) const
{
	auto l_it = m_resources.find(t_id);

	if (l_it == m_resources.end())
		return nullptr;

	return &l_it->second;
}

bool ResourceRegistry::isRegistered(uint32_t t_id) const
{
	return m_resources.find(t_id) != m_resources.end();
}

int ResourceRegistry::getCount() const
{
	return static_cast<int>(m_resources.size());
}

void ResourceRegistry::initializeResources()
{
	
	
	
	registerResource({ 1, "Wood", "Simply wood, seems sturdy.", 99, ResourceCategory::untapped });
	registerResource({ 2, "Stone", "Either these are pebbles or you are strong.", 99, ResourceCategory::untapped });
	registerResource({ 3, "Iron Ore", "Iron, a useful crafting material", 64, ResourceCategory::raw });
	registerResource({ 4, "Bucketed Water", "It's a bucket of water.", 1, ResourceCategory::untapped });
	registerResource({ 5, "Sand", "I hate sand, it gets everywhere...", 99, ResourceCategory::untapped });
	registerResource({ 6, "Dirt", "I've got a Jar of Dirt! I've got a Jar of Dirt!!", 99, ResourceCategory::untapped });
}
