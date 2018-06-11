/**
 * @file     log4pa.h
 * @Author   @seniorkot
 * @date     May, 2018
 * @brief    Header file for logger functions
 */

#ifndef __IFMO_DISTRIBUTED_CLASS_LOG3PA__H
#define __IFMO_DISTRIBUTED_CLASS_LOG3PA__H

#include "communication.h"
#include "banking.h"
#include "ipc.h"


void log_init();
void log_destroy();

void log_pipes(PipesCommunication* comm);

void log_started(local_id id, balance_t balance);
void log_received_all_started(local_id id);
void log_done(local_id id, balance_t balance);
void log_received_all_done(local_id id);

void log_transfer_out(local_id from, local_id dst, balance_t amount);
void log_transfer_in(local_id from, local_id dst, balance_t amount);

#endif
