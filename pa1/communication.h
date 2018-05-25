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

typedef struct{
	int* pipes;
	local_id current_id;
	size_t total_ids;
} PipesCommunication;

enum PipeTypeOffset 
{
    PIPE_READ_TYPE = 0,
    PIPE_WRITE_TYPE
};

int* pipes_init(size_t proc_count);
PipesCommunication* communication_init(int* pipes, size_t proc_count, local_id curr_proc);
void communication_destroy(PipesCommunication* comm);
