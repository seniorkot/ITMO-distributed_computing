/**
 * @file     main.c
 * @Author   @seniorkot
 * @date     May, 2018
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#include "pa1.h"
#include "ipc.h"
#include "log1pa.h"
#include "communication.h"

#define BUFFER_SIZE 512

int get_proc_count(char** argv);
int send_msg(PipesCommunication* comm, MessageType type);
int recieve_msgs(PipesCommunication* comm, MessageType type);

/**
 * @return -1 on invalid arguments, -2 on fork error, 0 on success
 */
int main(int argc, char** argv){
	size_t i;
	int proc_count;
	local_id current_proc_id;
	pid_t fork_id;
	pid_t* children;
	int* pipes;
	PipesCommunication* comm;
	
	/* Resolving program arguments */
	switch(argc){
		case 1:
			proc_count = 1; /* TODO: set new default value */
			break;
		case 3:
			proc_count = get_proc_count(argv);
			if (proc_count <= 0 || proc_count > MAX_PROCESS_ID + 1){
				fprintf(stderr, "Usage: %s -p (1-16)\n", argv[0]);
				return -1;
			}
			break;
		default:
			fprintf(stderr, "Usage: %s -p (1-16)\n", argv[0]);
			return -1;
	}
	
	/* Initialize log files */
	log_init();
	
	/* Allocate memory for children */
	children = malloc(sizeof(pid_t) * proc_count);
	
	/* Open pipes for all processes */
	pipes = pipes_init(proc_count + 1);
	
	/* Create children processes */
	for (i = 0; i < proc_count; i++){
		fork_id = fork();
		if (fork_id < 0){
			return -2;
		}
		else if (fork_id == 0){
			free(children);
			break;
		}
		children[i] = fork_id;
	}
	
	/* Set current process id */
	if (fork_id == 0){
		current_proc_id = i + 1;
	}
	else{
		current_proc_id = PARENT_ID;
	}
	
	/* Set pipe fds to process params */
	comm = communication_init(pipes, proc_count + 1, current_proc_id);
	log_pipes(comm);
	
	/* Send & recieve started message */
	if (current_proc_id != PARENT_ID){
		send_msg(comm, STARTED);
	}
	recieve_msgs(comm, STARTED);
	
	/* Send & recieve done message */
	if (current_proc_id != PARENT_ID){
		send_msg(comm, DONE);
	}
	recieve_msgs(comm, DONE);
	
	/* Waiting for all children if parent process */
	if (current_proc_id == PARENT_ID){
		for (i = 0; i < proc_count; i++){
			waitpid(children[i], NULL, 0);
		}
	}
	
	log_destroy();
	communication_destroy(comm);
	return 0;
}

/** Get process count from command line arguments.
 *
 * @param argv		Double char array containing command line arguments.
 *
 * @return -1 on error, any other values on success.
 */
int get_proc_count(char** argv){
	if (!strcmp(argv[1], "-p")){
		return atoi(argv[2]);
	}
	return -1;
}

/** Send message to all other processes
 *
 * @param comm		Pointer to PipesCommunication
 * @param type		Message type: STARTED / DONE
 *
 * @return -1 on incorrect type, -2 on internal error, -3 on sending message error, 0 on success.
 */
int send_msg(PipesCommunication* comm, MessageType type){
	Message* msg = malloc(sizeof(Message));
	uint16_t length = 0;
	char buf[BUFFER_SIZE];
    msg->s_header.s_magic = MESSAGE_MAGIC;
    msg->s_header.s_type = type;
    msg->s_header.s_local_time = time(NULL);
	
	type == STARTED ? log_started(comm->current_id) : log_done(comm->current_id);
	
	switch (type){
        case STARTED:
			length = snprintf(buf, BUFFER_SIZE, log_started_fmt, comm->current_id, getpid(), getppid());
			break;
		case DONE:
			length = snprintf(buf, BUFFER_SIZE, log_done_fmt, comm->current_id);
			break;
		default:
			return -1;
	}
	
	if (length <= 0){
		return -2;
	}
	
	msg->s_header.s_payload_len = length;
    memcpy(msg->s_payload, buf, sizeof(char) * length);
	
	if(send_multicast(comm, msg)){
		return -3;
	}
	
	free(msg);
	return 0;
}

/** Receive messages from other processes
 *
 * @param comm		Pointer to PipesCommunication
 * @param type		Message type: STARTED / DONE
 *
 * @return -1 on recieving message error, 0 on success.
 */
int recieve_msgs(PipesCommunication* comm, MessageType type){
	Message* msg = malloc(sizeof(Message));
	
	if (receive_any(comm, msg)){
		return -1;
	}
	
	switch (type){
        case STARTED:
            log_received_all_started(comm->current_id);
            break;
        case DONE:
            log_received_all_done(comm->current_id);
            break;
		default:
			break;
    }
	free(msg);
	return 0;
}
