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

void MachineRegistry::initializeMachines()
{
    registerMachine(MachineDefinition(
        1,
        "Simple Conveyor",
        "Simple but effective, can transport nearly all materials.",
        MachinePurpose::Transporter,
        InteractionType::None,
        0.0f,     
        0.5f,     
        1,        
        0,
        0,
        3,        
        2,        
        2         
    ));

    registerMachine(MachineDefinition(
        2,
        "Tree Chopper",
        "Perfect for planned deforestation.",
        MachinePurpose::Harvester,
        InteractionType::Chop,
        3.0f,   
        1.0f,   
        0,      
        2,      
        1,      
        0,      
        0,      
        3       
    ));

    registerMachine(MachineDefinition(
        3,
        "Auto Quarry",
        "Useful for mining Stone and Most Ores.",
        MachinePurpose::Harvester,
        InteractionType::Mine,
        4.0f,   
        0.0f,   
        0, 
        1,      
        0,      
        0,      
        0,      
        8       
    ));

    registerMachine(MachineDefinition(
        4,
        "Storage Chest",
        "Can store almost anything you can carry, with a Generous Capacity! Thank Corporate.",
        MachinePurpose::Storage,
        InteractionType::None,
        0.0f,
        0.0f,    
        0,       
        0,
        0,
        4,       
        16,      
        40       
    ));
}