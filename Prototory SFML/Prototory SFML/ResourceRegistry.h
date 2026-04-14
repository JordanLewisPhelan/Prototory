#pragma once

#include <unordered_map>
#include <iostream>
#include "ResourceDefinition.h"

class ResourceRegistry
{
public:
	
	void registerResource(ResourceDefinition t_resource);

	
	const ResourceDefinition* getResource(uint32_t t_id) const;

	
	bool isRegistered(uint32_t t_id) const;

	
	int getCount() const;

	
	void initializeResources();

private:
	
	std::unordered_map<uint32_t, ResourceDefinition> m_resources;
};