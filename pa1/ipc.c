/**
 * @file     ipc.c
 * @Author   @seniorkot
 * @date     May, 2018
 * @brief    IPC functions
 */

#include <unistd.h>
 
#include "ipc.h"
#include "communication.h"

#define GET_INDEX(x, id) ((x) < (id) ? (x) : (x) - 1)

int send(void * self, local_id dst, const Message * msg){
	PipesCommunication* from = (PipesCommunication*) self;
	
	if (dst == from->current_id){
		return -1;
	}
	if (write(from->pipes[GET_INDEX(dst, from->current_id) * 2 + PIPE_WRITE_TYPE], msg, sizeof(MessageHeader) + msg->s_header.s_payload_len) < 0){
		return -2;
	}
	return 0;
}

int send_multicast(void * self, const Message * msg){
	PipesCommunication* from = (PipesCommunication*) self;
	local_id i;
	
	for (i = 0; i < from->total_ids; i++){
		if (i == from->current_id){
			continue;
		}
		if (send(from, i, msg) < 0){
			return -1;
		}
	}
	return 0;
}

int receive(void * self, local_id from, Message * msg){
	PipesCommunication* this = (PipesCommunication*) self;
	
	if (from == this->current_id){
		return -1;
	}
	/* Read Header */
	if (read(this->pipes[GET_INDEX(from, this->current_id) * 2 + PIPE_READ_TYPE], msg, sizeof(MessageHeader)) < sizeof(MessageHeader)){
		return -2;
	}
	
	/* Read Body */
	if (read(this->pipes[GET_INDEX(from, this->current_id) * 2 + PIPE_READ_TYPE], ((char*) msg) + sizeof(MessageHeader), msg->s_header.s_payload_len) < 0){
		return -3;
	}
	return 0;
}

int receive_any(void * self, Message * msg){
	PipesCommunication* this = (PipesCommunication*) self;
	local_id i;
	
	for (i = 1; i < this->total_ids; i++){
		if (i == this->current_id){
			continue;
		}
		
		if (receive(this, i, msg) != 0){
			return -1;
		}
	}
	return 0;
}
