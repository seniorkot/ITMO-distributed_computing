/**
 * @file     lamport.h
 * @Author   @seniorkot
 * @date     June, 2018
 * @brief    Implementation of lamport time functions
 */
 
#include "lamport.h"

#include <stdlib.h>

static timestamp_t lamport_time = 0;

/** Compare 2 QueueNodes
 *
 * @param one
 * @param two
 *
 * @return -1 if two is greater, 1 if one is greater, 0 on equal.
 */
int node_cmp(QueueNode* one, QueueNode* two){
	if (one->key < two->key){
		return -1;
	}
	if (one->key > two->key){
		return 1;
	}
    if (one->value < two->value){
		return -1;
	}
    if (one->value > two->value){
		return 1;
	}
	return 0;
}

LamportQueue* lamport_queue_init(){
	LamportQueue* queue = malloc(sizeof(LamportQueue));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

void lamport_queue_destroy(LamportQueue* queue){
	if (queue->head != NULL){
		QueueNode* node = queue->head;
		do{
			QueueNode* next = node->next;
			free(node);
			node = next;
		} while(node != NULL);
	}
	free(queue);
}

void lamport_queue_insert(LamportQueue* queue, timestamp_t key, local_id value){
	QueueNode* node = malloc(sizeof(QueueNode));
	node->key = key;
	node->value = value;
	node->prev = NULL;
	node->next = NULL;
	
	/* If queue is empty */
	if (queue->head == NULL){
		queue->head = node;
		queue->tail = node;
	}
	/* If node is greater than queue head element */
	else if (node_cmp(node, queue->head) < 0){
		queue->head->prev = node;
		node->next = queue->head;
		queue->head = node;
	}
	/* If node is lower than queue tail element */
	else if (node_cmp(node, queue->tail) > 0){
		queue->tail->next = node;
		node->prev = queue->tail;
		queue->tail = node;
	}
	else{
		QueueNode* tmp_left = queue->head;		/* Tmp nodes for inserting in queue */
		QueueNode* tmp_right = queue->tail;
		
		while(!(node_cmp(node, tmp_left) > 0 && node_cmp(node, tmp_right) < 0)){
			tmp_left = tmp_right;
			tmp_right = tmp_left->next;
		}
		
		node->prev = tmp_left;
		node->next = tmp_right;
		tmp_left->next = node;
		tmp_right->prev = node;
	}	
}

local_id lamport_queue_peek(LamportQueue* queue){
	if (queue->head == NULL){
		return -1;
	}
	return queue->head->value;
}

local_id lamport_queue_get(LamportQueue* queue){
	QueueNode* tmp_head;
	local_id retval = lamport_queue_peek(queue);
	if (retval < 0){
		return -1;
	}
	
	tmp_head = queue->head->next;
	if (tmp_head != NULL){
		tmp_head->prev = NULL;
	}
	
	free(queue->head);
	queue->head = tmp_head;
	return retval;
}

/* Time functions */
timestamp_t increment_lamport_time(){
	return ++lamport_time;
}

timestamp_t set_lamport_time(timestamp_t new_lamport_time){
	if (lamport_time < new_lamport_time){
		lamport_time = new_lamport_time;
	}
	return lamport_time;
}

timestamp_t set_lamport_time_from_msg(Message* msg){
	set_lamport_time(msg->s_header.s_local_time);
	return increment_lamport_time();
}

timestamp_t get_lamport_time(){
	return lamport_time;
}
