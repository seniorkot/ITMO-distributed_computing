/**
 * @file     banking.c
 * @Author   @seniorkot
 * @date     May, 2018
 * @brief    Transfer function
 */
 
#include "banking.h"
#include "communication.h"
#include "log3pa.h"
#include "ltime.h"

void transfer(void * parent_data, local_id src, local_id dst, balance_t amount){
	Message msg;
	PipesCommunication* parent = (PipesCommunication*) parent_data;
	TransferOrder order;
	order.s_src = src;
    order.s_dst = dst;
    order.s_amount = amount;
	
	increment_lamport_time();
	
    send_transfer_msg(parent, src, &order);
	
	log_transfer_out(src, dst, amount);
		
    while (receive(parent, dst, &msg) < 0 || msg.s_header.s_type != ACK);
	
	set_lamport_time_from_msg(&msg);
	
	log_transfer_in(src, dst, amount);		
}
