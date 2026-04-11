#include "MachineRegistry.h"


void MachineRegistry::registerMachine(MachineDefinition t_machine)
{
    if (isRegistered(t_machine.m_id))
    {
        std::cout << "MachineRegistry: Warning - ID: " << t_machine.m_id
            << " already registered, ignoring.\n\n";
        return;
    }
    m_machineDefinitions.emplace(t_machine.m_id, t_machine);
    std::cout << "MachineRegistry: Machine ["
            << t_machine.m_id << "] "
            << t_machine.m_name << "\n\n";
}

const MachineDefinition* MachineRegistry::getMachine(uint32_t t_id) const
{
    auto l_it = m_machineDefinitions.find(t_id);
    if (l_it == m_machineDefinitions.end())
        return nullptr;
    return &l_it->second;
}

bool MachineRegistry::isRegistered(uint32_t t_id) const
{
    return m_machineDefinitions.find(t_id) != m_machineDefinitions.end();
}

int MachineRegistry::getCount() const
{
    return static_cast<int>(m_machineDefinitions.size());
}


/*
    Default values currently
*/
void MachineRegistry::initializeMachines()
{
    registerMachine(MachineDefinition(
        1,
        "Simple Conveyor",
        "Simple but effective, can transport nearly all materials.",
        MachinePurpose::Transporter,
        InteractionType::None,
        0.0f,     // No logic ticks
        0.5f,     // Transfers items over 0.5 seconds
        1,        // Amount of Items transferred
        0,
        0,
        3,        // Accepts up to 3 inputs
        2,        // Each channel has 2 buffer slots
        2         // 2 Inventory slots - was to be 1 but allowance for different minerals together may be worth considering?
    ));

    registerMachine(MachineDefinition(
        2,
        "Tree Chopper",
        "Perfect for planned deforestation.",
        MachinePurpose::Harvester,
        InteractionType::Chop,
        3.0f,   // harvests every 3 seconds
        1.0f,   // slow 1 second transfer 
        0,      
        2,      // 1 unit per harvest
        1,      // 1 tile radius around the harvester
        0,      // nothing feeds into a harvester
        0,      // no channel buffers
        3       // output inventory size
    ));

    registerMachine(MachineDefinition(
        3,
        "Auto Quarry",
        "Useful for mining Stone and Most Ores.",
        MachinePurpose::Harvester,
        InteractionType::Mine,
        4.0f,   // harvests every 4 seconds
        0.0f,   // no transfer tick
        0, 
        1,      // 1 unit per harvest
        0,      // Local tile mining only with this harvester
        0,      // nothing feeds into a harvester
        0,      // no channel buffers
        8       // output inventory size
    ));

    registerMachine(MachineDefinition(
        4,
        "Storage Chest",
        "Can store almost anything you can carry, with a Generous Capacity! Thank Corporate.",
        MachinePurpose::Storage,
        InteractionType::None,
        0.0f,
        0.0f,    // We only accept inputs, no automatic transfers - *Yet*
        0,       // Amount of Items transferred *Chests wont use this innately yet - just for later* 
        0,
        0,
        4,       // Accepts inputs from 4 directions
        16,      // Sizeable Buffer channel to account for multiple inputs
        40       // Large storage capacity
    ));
}