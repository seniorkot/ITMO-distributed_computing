/**
 * @file     cs.h
 * @Author   @seniorkot
 * @date     June, 2018
 * @brief    Header file for CS structure
 */
 
#ifndef __IFMO_DISTRIBUTED_CLASS_CS__H
#define __IFMO_DISTRIBUTED_CLASS_CS__H

#include "communication.h"
#include "lamport.h"
#include "ipc.h"

typedef struct{
	PipesCommunication* comm;
	LamportQueue* queue;
	size_t done_left;
} CS;

int cs_work(CS* lamport_comm, Message* msg);
 
#endif
