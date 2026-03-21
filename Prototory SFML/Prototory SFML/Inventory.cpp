#include "Inventory.h"


Inventory::Inventory(int t_slotCount)
{
	m_slots.resize(t_slotCount);
	std::cout << "Inventory: Created with " << t_slotCount << " slots.\n\n";
}

int Inventory::addResources(uint32_t t_resourceID, int t_amount, int t_maxStack)
{
	int l_remaining = t_amount;

	// Find all existing slots with this resource that have space
	for (int i = 0; i < static_cast<int>(m_slots.size()); i++)
	{
		if (l_remaining <= 0)
			break;

		InventorySlot& l_slot = m_slots[i];

		if (l_slot.m_isEmpty || l_slot.m_resourceID != t_resourceID)
			continue;

		int l_spaceInSlot = t_maxStack - l_slot.m_stackCount;
		if (l_spaceInSlot <= 0)
			continue;

		int l_toAdd = std::min(l_remaining, l_spaceInSlot);
		l_slot.m_stackCount += l_toAdd;
		l_remaining -= l_toAdd;
	}
	// Handle leftovers / Need for more slots
	while (l_remaining > 0)
	{
		int l_emptySlot = findEmptySlot();

		if (l_emptySlot == -1)
		{
			std::cout << "Inventory: Full - " << l_remaining << " resources could not be added.\n\n";
			return l_remaining;
		}

		InventorySlot& l_slot = m_slots[l_emptySlot];
		int l_toAdd = std::min(l_remaining, t_maxStack);
		l_slot.m_stackCount += l_toAdd;
		l_slot.m_resourceID = t_resourceID;
		l_slot.m_isEmpty = false;
		l_remaining -= l_toAdd;
	}

	return 0;
}

int Inventory::removeResources(uint32_t t_resourceID, int t_amount)
{
	int l_remaining = t_amount;

	for (InventorySlot& l_slot : m_slots)
	{
		if (l_slot.m_isEmpty || l_slot.m_resourceID != t_resourceID)
			continue;
		
		int l_toRemove = std::min(l_remaining, l_slot.m_stackCount);
		l_slot.m_stackCount -= l_toRemove;
		l_remaining -= l_toRemove;

		if (l_slot.m_stackCount == 0)
		{
			l_slot.m_resourceID = 0;
			l_slot.m_isEmpty = true;
		}

		if (l_remaining == 0)
			break;
	}

	return t_amount - l_remaining;
}

int Inventory::getResourceCount(uint32_t t_resourceID) const
{
	int l_total = 0;

	for (const InventorySlot& l_slot : m_slots)
	{
		if (!l_slot.m_isEmpty && l_slot.m_resourceID == t_resourceID)
			l_total += l_slot.m_stackCount;
	}

	return l_total;
}

void Inventory::transferTo(Inventory& t_targetInventory, uint32_t t_resourceID, int t_amount, int t_maxStack)
{
	int l_removed = removeResources(t_resourceID, t_amount);
	int l_leftOver = t_targetInventory.addResources(t_resourceID, l_removed, t_maxStack);

	if (l_leftOver > 0)
	{
		addResources(t_resourceID, l_leftOver, t_maxStack);
		std::cout << "Inventory: Partial Transfer [" << l_leftOver << "] resources returned to original inventory.\n\n";
	}
}

bool Inventory::isFull() const
{
	return findEmptySlot() != -1;
}

bool Inventory::isEmpty() const
{
	for (const InventorySlot& l_slot : m_slots)
	{
		if (!l_slot.m_isEmpty)
			return false;
	}

	return true;
}

int Inventory::getSlotCount() const
{
	return static_cast<int>(m_slots.size());
}

const std::vector<InventorySlot>& Inventory::getSlots() const
{
	return m_slots;
}

int Inventory::findSlotWithResource(uint32_t t_resourceID) const
{
	for (int i = 0; i < static_cast<int>(m_slots.size()); i++)
	{
		if (!m_slots[i].m_isEmpty && m_slots[i].m_resourceID == t_resourceID)
			return i;
	}
	return -1;
}

int Inventory::findEmptySlot() const
{
	for (int i = 0; i < static_cast<int>(m_slots.size()); i++)
	{
		if (m_slots[i].m_isEmpty)
			return i;
	}
	return -1;
}
