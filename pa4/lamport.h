/**
 * @file     lamport.h
 * @Author   @seniorkot
 * @date     May, 2018
 * @brief    Header file for lamport time functions
 */

#ifndef __IFMO_DISTRIBUTED_CLASS_LAMPORT__H
#define __IFMO_DISTRIBUTED_CLASS_LAMPORT__H

#include "ipc.h"

timestamp_t increment_lamport_time();
timestamp_t set_lamport_time(timestamp_t new_lamport_time);
timestamp_t set_lamport_time_from_msg(Message* msg);
timestamp_t get_lamport_time();

#endif
