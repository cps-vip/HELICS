/*
Copyright � 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/
#pragma once
/**
@file
operations and helper functions for handling flags in helics
*/
#include <cstdint>

/** flag definitions for the action Message Flag field*/
enum operation_flags : uint16_t
{
    iteration_requested_flag = 0,  //!< indicator that an iteration has been requested
    destination_target = 1,  //!< indicator that the target is a destination target
    required_flag = 2,  //!< flag indicating that an action or match is required
    core_flag = 3,  //!< flag indicating that message comes from a core vs a broker
    error_flag = 4,  //!< flag indicating an error condition associated with the command
    indicator_flag = 5,  //!< flag used for setting values
    empty_flag = 6,  //!< flag indicating that the message is empty
    extra_flag1 = 7,  //!< extra flag
    forwarding_coordinator = 8,  //!< flag indicating that a dependency is a forwarding coordinator
    clone_flag = 9,  //!< flag indicating the filter is a clone filter
    extra_flag2 = 8,  //!< extra flag
    destination_processing_flag = 11,  //!< flag indicating the message is for destination processing
    broker_min_time_flag = 12,  //!< flag indicating that a broker the time constraint
    extra_flag3 = 13,  //!< extra flag
    extra_flag4 = 14,  //!< extra flag
    nameless_interface_flag = 15,  //!< flag indicating the interface is nameless
};

/** template function to set a flag in an object containing a flags field*/
template <class FlagContainer, class FlagIndex>
inline void setActionFlag (FlagContainer &M, FlagIndex flag)
{
    M.flags |= (static_cast<decltype (M.flags)> (1) << (flag));
}

/** template function to check a flag in an object containing a flags field*/
template <class FlagContainer, class FlagIndex>
inline bool checkActionFlag (const FlagContainer &M, FlagIndex flag)
{
    return ((M.flags & (static_cast<decltype (M.flags)> (1) << (flag))) != 0);
}

/** template function to clear a flag in an object containing a flags field*/
template <class FlagContainer, class FlagIndex>
inline void clearActionFlag (FlagContainer &M, FlagIndex flag)
{
    M.flags &= ~(static_cast<decltype (M.flags)> (1) << (flag));
}


inline constexpr uint16_t make_flags (int flag) { return static_cast<uint16_t> (1) << (flag); }

inline constexpr uint16_t make_flags (int flag1, int flag2) { return make_flags (flag1) | make_flags (flag2); }

inline constexpr uint16_t make_flags (int flag1, int flag2, int flag3)
{
    return make_flags (flag1, flag2) | make_flags (flag3);
}