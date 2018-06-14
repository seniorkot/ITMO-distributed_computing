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
 * @return -1 if @one comes first, 1 if @two comes first, 0 on equal.
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

/** Init Lamport Queue
 * 
 * @return 	pointer to LamportQueue
 */
LamportQueue* lamport_queue_init(){
	LamportQueue* queue = malloc(sizeof(LamportQueue));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

/** Destroy Lamport Queue
 * 
 * @param queue 	pointer to LamportQueue
 */
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

/** Insert value into queue <key, value>
 * 
 * @param queue 	pointer to LamportQueue
 * @param key	 	Lamport timestamp
 * @param value 	Process local_id
 */
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
		node->next = queue->head;
		queue->head->prev = node;
		queue->head = node;
	}
	/* If node is lower than queue tail element */
	else if (node_cmp(node, queue->tail) > 0){
		node->prev = queue->tail;
		queue->tail->next = node;
		queue->tail = node;
	}
	else{
		QueueNode* tmp_left = queue->head;		/* Tmp nodes for inserting in queue */
		QueueNode* tmp_right = queue->head->next;
		
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

/** Get head value from queue without deleting it
 * 
 * @param queue 	pointer to LamportQueue
 *
 * @return head value
 */
local_id lamport_queue_peek(LamportQueue* queue){
	if (queue->head == NULL){
		return -1;
	}
	return queue->head->value;
}

/** Get head value and delete it from queue
 * 
 * @param queue 	pointer to LamportQueue
 *
 * @return head value
 */
local_id lamport_queue_get(LamportQueue* queue){
	QueueNode* tmp_head;
	local_id retval;
	if ((retval = lamport_queue_peek(queue)) < 0){
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
	lamport_time++;
	return lamport_time;
}

timestamp_t set_lamport_time(timestamp_t new_lamport_time){
	if (lamport_time < new_lamport_time){
		lamport_time = new_lamport_time;
	}
	return lamport_time;
}

timestamp_t set_lamport_time_from_msg(Message* msg){
	set_lamport_time(msg->s_header.s_local_time);
	return lamport_time;
}

timestamp_t get_lamport_time(){
	return lamport_time;
}
