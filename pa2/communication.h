/**
 * @file     communication.h
 * @Author   @seniorkot
 * @date     May, 2018
 * @brief    Header file that contains structure and functions to organize IPC
 */

#ifndef __IFMO_DISTRIBUTED_CLASS_COMMUNICATION__H
#define __IFMO_DISTRIBUTED_CLASS_COMMUNICATION__H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "ipc.h"
#include "banking.h"

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
void send_all_stop_msg(PipesCommunication* comm);
void send_transfer_msg(PipesCommunication* comm, local_id dst, TransferOrder* order);
void send_ack_msg(PipesCommunication* comm, local_id dst);
void send_balance_history(PipesCommunication* comm, local_id dst, BalanceHistory* history);

void receive_all_msgs(PipesCommunication* comm, MessageType type);

#endif
