/**
 * @file     main.c
 * @Author   @seniorkot
 * @date     June, 2018
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <getopt.h>

#include "log4pa.h"
#include "communication.h"
#include "lamport.h"
#include "cs.h"
#include "pa2345.h"

int get_agrs(int argc, char** argv, int* processes, int* mutexl);

int do_parent_work(PipesCommunication* comm);
int do_child_work(PipesCommunication* comm, int mutexl);

/**
 * @return -1 on invalid arguments, -2 on fork error, 0 on success
 */
int main(int argc, char** argv){
	size_t i;
	int proc_count;
	int mutexl;
	int* pipes;
	pid_t* children;
	pid_t fork_id;
	local_id current_proc_id;
	PipesCommunication* comm;
	
	/* Check args */
	if (argc < 3 || get_agrs(argc, argv, &proc_count, &mutexl) == -1){
		fprintf(stderr, "Usage: %s -p X [--mutexl]\n", argv[0]);
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
		else if (!fork_id){
			free(children);
			break;
		}
		children[i] = fork_id;
	}
	
	/* Set current process id */
	if (!fork_id){
		current_proc_id = i + 1;
	}
	else{
		current_proc_id = PARENT_ID;
	}
	
	/* Set pipe fds to process params */
	comm = communication_init(pipes, proc_count + 1, current_proc_id);
	log_pipes(comm);
	
	/* Do process work */
	if (current_proc_id == PARENT_ID){
		do_parent_work(comm);
	}
	else{
		do_child_work(comm, mutexl);
	}
	
	/* Waiting for all children if parent process */
	if (current_proc_id == PARENT_ID){
		for (i = 0; i < proc_count; i++){
			waitpid(children[i], NULL, 0);
		}
	}
	
	/* Finish work */
	log_destroy();
	communication_destroy(comm);
	return 0;
}

/** Do parent process work
 *
 * @param comm		Pointer to PipesCommunication
 *
 * @return -1 on error, 0 on success.
 */
int do_parent_work(PipesCommunication* comm){
	CS lamport_comm;
	lamport_comm.comm = comm;
	lamport_comm.queue = NULL;
	lamport_comm.done_left = comm->total_ids - 1;
	
	/* Receive STARTED messages from children */
	receive_all_msgs(comm, STARTED);
	
	/* Receive messages: wait for all children done */
	while (lamport_comm.done_left){
		Message msg;
		
		while (receive_any(comm, &msg));
		
		/* Ignore REQUEST & RELEASE messages */
		if (msg.s_header.s_type == DONE){
			set_lamport_time_from_msg(&msg);
			cs_work(&lamport_comm, &msg);
		}
	}
	
	log_received_all_done(comm->current_id);
	return 0;
}

/** Do child process work
 *
 * @param comm		Pointer to PipesCommunication
 * @param mutexl	Mutexl flag
 *
 * @return -1 on error, 0 on success.
 */
int do_child_work(PipesCommunication* comm, int mutexl){
	LamportQueue* queue = lamport_queue_init();
	CS lamport_comm;
	local_id i;
	char buf[MAX_PAYLOAD_LEN];
	
	lamport_comm.comm = comm;
	lamport_comm.queue = queue;
	lamport_comm.done_left = comm->total_ids - 2;
	
	/* Send & receive STARTED messages */
	send_all_proc_event_msg(comm, STARTED);
	receive_all_msgs(comm, STARTED);
	
	/* Do process work */
	for (i = 1; i <= comm->current_id * 5; i++){
		/* If "--mutexl" is set, request entering critical area */
		if (mutexl){
			request_cs(&lamport_comm);
		}
		/* Critical area */
		snprintf(buf, MAX_PAYLOAD_LEN, log_loop_operation_fmt, comm->current_id, i, comm->current_id * 5);
		print(buf);
		
		/* If "--mutexl" is set, notify all about exiting critical area */
		if (mutexl){
			release_cs(&lamport_comm);
		}
	}
	
	/* Notify all that process is done */
	send_all_proc_event_msg(comm, DONE);
	
	/* Receive messages: wait for all done, reply on requests */
	while (lamport_comm.done_left){
		Message msg;
		
		while (receive_any(comm, &msg));
		
		set_lamport_time_from_msg(&msg);
		cs_work(&lamport_comm, &msg);
	}
	log_received_all_done(comm->current_id);
	
	lamport_queue_destroy(queue);
	return 0;
}

/** Get program arguments
 *
 * @param argc			Arguments count
 * @param argv			Double char array containing command line arguments
 * @param processes		Pointer to proc_count variable
 * @param mutexl		Pointer to mutexl flag variable
 *
 * @return -1 on error, 0 on success.
 */
int get_agrs(int argc, char** argv, int* processes, int* mutexl){
	int res;
	const struct option long_options[] = {
        {"mutexl", no_argument, mutexl, 1},
        {NULL, 0, NULL, 0}
    };
	
	*mutexl = 0;
	
	while ((res = getopt_long(argc, argv, "p:", long_options, NULL)) != -1){
		if (res == 'p'){
			*processes = atoi(optarg);
		}
		else if (res == '?'){
			return -1;
		}
	}
	return 0;
}
