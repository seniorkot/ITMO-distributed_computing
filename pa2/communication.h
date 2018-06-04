/**
 * @file     communication.h
 * @Author   @seniorkot
 * @date     May, 2018
 * @brief    Header file that contains structure and functions to organize IPC
 */

#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "ipc.h"
#include "balance.h"

typedef struct{
	int* pipes;
	local_id current_id;
	size_t total_ids;
	balance_t balance;
} PipesCommunication;

enum PipeTypeOffset 
{
    PIPE_READ_TYPE = 0,
    PIPE_WRITE_TYPE
};

int* pipes_init(size_t proc_count);
PipesCommunication* communication_init(int* pipes, size_t proc_count, local_id curr_proc, balance_t balance);
void communication_destroy(PipesCommunication* comm);

int send_all_proc_event_msg(PipesCommunication* comm, MessageType type);
int send_all_stop_msg(PipesCommunication* comm);
int send_transfer_msg(PipesCommunication* comm, local_id dst, TransferOrder* order);
int send_ack_msg(PipesCommunication* comm, local_id dst);

int receive_all_msgs(PipesCommunication* comm, MessageType type);
