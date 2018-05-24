/**
 * @file     main.c
 * @Author   Oleg Ivanko (@seniorkot) & Martin Rayla
 * @date     May, 2018
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "pa1.h"
#include "ipc.h"
#include "log1pa.h"
#include "communication.h"

int get_proc_count(char** argv);

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
		else if (fork_id){
			free(children);
			break;
		}
		children[i] = fork_id;
	}
	
	/* Set current process id */
	if (fork_id != 0){
		current_proc_id = i + 1;
	}
	else{
		current_proc_id = PARENT_ID;
	}
	
	comm = communication_init(pipes, proc_count + 1, current_proc_id);
	log_pipes(comm);
	
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
