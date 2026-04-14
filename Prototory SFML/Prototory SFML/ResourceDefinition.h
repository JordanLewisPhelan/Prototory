#pragma once

#include <string>
#include <cstdint>
#include "Globals.h"

enum class ResourceCategory
{
	untapped, raw, processed, misc
};

struct ResourceDefinition {
	uint32_t m_id;
	std::string m_name;
	std::string m_description;
	int m_maxInStack;
	ResourceCategory m_category;

	ResourceDefinition(uint32_t t_id, std::string t_name, std::string t_description
						, int t_maxInStack, ResourceCategory t_category)
		: m_id(t_id), m_name(t_name), m_description(t_description)
		, m_maxInStack(t_maxInStack), m_category(t_category)
		{}
};