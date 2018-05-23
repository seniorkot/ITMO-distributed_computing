/**
 * @file     log1pa.h
 * @Author   Oleg Ivanko (@seniorkot) & Martin Rayla
 * @date     May, 2018
 * @brief    Header file for logger functions
 */

#pragma once

#include <stdio.h>
#include <process.h>
#include "common.h"
#include "ipc.h"
#include "pa1.h"

FILE* pipes_log_f;
FILE* events_log_f;

void log_init();
void log_started(local_id id);
void log_received_all_started(local_id id);
void log_done(local_id id);
void log_received_all_done(local_id id);
void log_destroy();