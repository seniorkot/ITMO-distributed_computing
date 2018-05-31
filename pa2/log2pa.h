/**
 * @file     log2pa.h
 * @Author   @seniorkot
 * @date     May, 2018
 * @brief    Header file for logger functions
 */

#pragma once

#include <stdio.h>
#include <unistd.h>

#include "banking.h"
#include "common.h"
#include "ipc.h"
#include "pa2345.h"
#include "communication.h"

void log_init();
void log_destroy();

void log_pipes(PipesCommunication* comm);

void log_started(local_id id, balance_t balance);
void log_received_all_started(local_id id);
void log_done(local_id id, balance_t balance);
void log_received_all_done(local_id id);

void log_transfer_out(local_id from, local_id dst, balance_t amount);
void log_transfer_in(local_id from, local_id dst, balance_t amount);
