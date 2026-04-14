#pragma once

#include <string>
#include <cstdint>
#include "Globals.h"

enum class MachinePurpose
{
    Harvester,
    Transporter,
    Storage,
    Processor       
};

struct MachineDefinition
{
    uint32_t        m_id;
    std::string     m_name;
    std::string     m_description;
    MachinePurpose  m_purpose;
    InteractionType m_harvestType;    
    float           m_tickRate;       
    float           m_transferRate;   
    int             m_transferAmount; 
    int             m_harvestAmount;
    int             m_harvestRange;   
    int             m_inputChannels;  
    int             m_inputSlots;     
    int             m_outputSlots;    

    MachineDefinition(
        uint32_t        t_id,
        std::string     t_name,
        std::string     t_description,
        MachinePurpose  t_purpose,
        InteractionType t_harvestType,
        float           t_tickRate,
        float           t_transferRate,
        int             t_transferAmount,
        int             t_harvestAmount,
        int             t_harvestRange,
        int             t_inputChannels,
        int             t_inputSlots,
        int             t_outputSlots
    )
        : m_id(t_id)
        , m_name(t_name)
        , m_description(t_description)
        , m_purpose(t_purpose)
        , m_harvestType(t_harvestType)
        , m_tickRate(t_tickRate)
        , m_transferRate(t_transferRate)
        , m_transferAmount(t_transferAmount)
        , m_harvestAmount(t_harvestAmount)
        , m_harvestRange(t_harvestRange)
        , m_inputChannels(t_inputChannels)
        , m_inputSlots(t_inputSlots)
        , m_outputSlots(t_outputSlots)
    {}
};