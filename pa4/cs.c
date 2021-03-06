/**
 * @file     cs.c
 * @Author   @seniorkot
 * @date     June, 2018
 * @brief    Implementation of CS functions
 */
 
#include "cs.h"
#include "pa2345.h"

int request_cs(const void * self){
	CS* lamport_comm = (CS*) self;
	PipesCommunication* comm = lamport_comm->comm;
	LamportQueue* queue = lamport_comm->queue;
	Message msg;
	size_t reply_left = comm->total_ids - 2;
	
	/* Step 1: Inserting self into the queue. */
	lamport_queue_insert(queue, get_lamport_time(), comm->current_id);
	send_all_request_msg(comm);
	
	/* Step 2: Receiving messages - inserting others into the queue / receiveng replies. */
	while (reply_left){
		while (receive_any(comm, &msg));
		
		set_lamport_time_from_msg(&msg);
		
		cs_work(lamport_comm, &msg);
		
		if (msg.s_header.s_type == CS_REPLY){
            reply_left--;
        }
	}
	
	/* Step 3: Waiting for process turn. Exit function. */
	while (lamport_queue_peek(queue) != comm->current_id){
		while (receive_any(comm, &msg));
		
		set_lamport_time_from_msg(&msg);
		
		cs_work(lamport_comm, &msg);
	}
	
	return 0;
}

int release_cs(const void * self){
	CS* lamport_comm = (CS*) self;
	PipesCommunication* comm = lamport_comm->comm;
	LamportQueue* queue = lamport_comm->queue;
	
	send_all_release_msg(comm);
	lamport_queue_get(queue);
	return 0;
}

int cs_work(CS* lamport_comm, Message* msg){
	PipesCommunication* comm = lamport_comm->comm;
	LamportQueue* queue = lamport_comm->queue;
	
	if (msg->s_header.s_type == CS_REQUEST){
        lamport_queue_insert(queue, msg->s_header.s_local_time - 1, comm->last_msg_from);

        send_reply_msg(comm, comm->last_msg_from);
    }
    else if (msg->s_header.s_type == CS_RELEASE){
        if (lamport_queue_get(queue) != comm->last_msg_from){
            return -1;
        }
    }
	else if (msg->s_header.s_type == DONE){
		lamport_comm->done_left--;
	}
	return 0;
}
