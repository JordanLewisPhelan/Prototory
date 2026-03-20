#pragma once

#include <algorithm>
#include <vector>
#include <cstdint>
#include <iostream>


struct InventorySlot
{
	uint32_t m_resourceID{ 0 };
	int m_stackCount{ 0 };
	bool isEmpty{ true };
};


class Inventory
{
public:
	Inventory(int t_slotCount);

	/* - Inventory Slot Influencers - */
	// Adds a given amount of resources, returns what is left over
	int addResources(uint32_t t_resourceID, int t_amount, int t_maxStack);

	// Removes a given amount of resources, returns amount removed successfully
	int removeResources(uint32_t t_resourceID, int t_amount);

	int getResourceCount(uint32_t t_resourceID) const;

	// Transfer resources from this Inventory to another
	void transferTo(Inventory& t_targetInventory, uint32_t t_resourceID, int t_amount, int t_maxStack);

	/* - Queries - */
	bool isFull() const;
	bool isEmpty() const;
	int getSlotCount() const;
	const std::vector<InventorySlot>& getSlots() const;

private:
	std::vector<InventorySlot> m_slots;

	// Returns the first existing slot with a given resource
	int findSlotWithResource(uint32_t t_resourceID) const;
	// Returns index of first empty slot - -1 if all are full
	int findEmptySlot() const;
};