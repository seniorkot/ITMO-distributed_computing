/**
 * @file     banking.c
 * @Author   @seniorkot
 * @date     May, 2018
 * @brief    Transfer function
 */
#include <unistd.h>
#include <stdlib.h>
 
#include "banking.h"
#include "communication.h"
#include "log2pa.h"

void transfer(void * parent_data, local_id src, local_id dst, balance_t amount){
	PipesCommunication* parent = (PipesCommunication*) parent_data;
	TransferOrder* order = malloc(sizeof(TransferOrder));
	order->s_src = src;
    order->s_dst = dst;
    order->s_amount = amount;
	
	send_transfer_msg(parent, dst, order);
	log_transfer_out(src, dst, amount);
	
	Message* msg = malloc(sizeof(Message));
	
	while (receive(parent, dst, msg) <= 0);
	
	if (msg->s_header.s_type == ACK){
		log_transfer_in(src, dst, amount);		
	}
	free(msg);
}
