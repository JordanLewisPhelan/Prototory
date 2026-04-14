#pragma once

#include <algorithm>
#include <vector>
#include <cstdint>
#include <iostream>

struct InventorySlot
{
	uint32_t m_resourceID{ 0 };
	int m_stackCount{ 0 };
	bool m_isEmpty{ true };
};

class Inventory
{
public:
	Inventory(int t_slotCount);

	
	
	int addResources(uint32_t t_resourceID, int t_amount, int t_maxStack);

	
	int removeResources(uint32_t t_resourceID, int t_amount);

	int getResourceCount(uint32_t t_resourceID) const;

	
	void transferTo(Inventory& t_targetInventory, uint32_t t_resourceID, int t_amount, int t_maxStack);

	
	bool isFull() const;
	bool isEmpty() const;
	int getSlotCount() const;
	const std::vector<InventorySlot>& getSlots() const;

private:
	std::vector<InventorySlot> m_slots;

	
	int findSlotWithResource(uint32_t t_resourceID) const;
	
	int findEmptySlot() const;
};