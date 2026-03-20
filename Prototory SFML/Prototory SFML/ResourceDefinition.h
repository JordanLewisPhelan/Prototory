#pragma once

#include <string>
#include <cstdint>
#include "Globals.h"


/* -
	Categories to Identify a Resources Material stage, e.g. processed metals will be used far more often for
	machinery than raw metals and can allow us to use Processed materials over just Iron or Copper restrictively
- */
/// Note: Currently Not relevant to project but is literally a 60 second type up for a reminder
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