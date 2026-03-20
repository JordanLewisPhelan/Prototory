#pragma once

#include <unordered_map>
#include <iostream>
#include "ResourceDefinition.h"


class ResourceRegistry
{
public:
	// Registers a new resource definition
	void registerResource(ResourceDefinition t_resource);

	// Returns a resource based on inputted id
	const ResourceDefinition* getResource(uint32_t t_id) const;

	// Checks if an resource id exists
	bool isRegistered(uint32_t t_id) const;

	//	Returns total amount of registered resources
	int getCount() const;

	// Initializes resource defaults 
	void initializeResources();

private:
	// Faster look-up than a map
	std::unordered_map<uint32_t, ResourceDefinition> m_resources;
};