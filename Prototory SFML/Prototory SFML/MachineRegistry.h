#pragma once
#include <unordered_map>
#include <cstdint>
#include <iostream>
#include "MachineDefinition.h"

class MachineRegistry
{
public:
    void initializeMachines();
    void registerMachine(MachineDefinition t_definition);

    const MachineDefinition* getMachine(uint32_t t_id) const;
    bool isRegistered(uint32_t t_id) const;
    int getCount() const;

private:
    std::unordered_map<uint32_t, MachineDefinition> m_machineDefinitions;
};