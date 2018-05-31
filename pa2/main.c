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

#include "log2pa.h"
#include "communication.h"
#include "banking.h"

int get_proc_count(int argc, char** argv);
balance_t get_proc_balance(local_id proc_id, char** argv);

void do_parent_work(PipesCommunication* comm);
void do_child_work(PipesCommunication* comm);

/**
 * @return -1 on invalid arguments, -2 on fork error, 0 on success
 */
int main(int argc, char** argv){
	size_t i;
	int proc_count;
	int* pipes;
	pid_t* children;
	pid_t fork_id;
	local_id current_proc_id;
	PipesCommunication* comm;
	
	/* Check args */
	if (argc < 4 || (proc_count = get_proc_count(argv)) == -1){
		fprintf(stderr, "Usage: %s -p X y1 y2 ... yX\n", argv[0]);
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
	comm = communication_init(pipes, proc_count + 1, current_proc_id, get_proc_balance(argv, current_proc_id));
	log_pipes(comm);
	
	/* Do process work */
	if (current_proc_id == PARENT_ID){
		do_parent_work(comm);
	}
	else{
		do_child_work(comm);
	}
	
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

void do_parent_work(PipesCommunication* comm){
	
}

void do_child_work(PipesCommunication* comm){
	
}

/** Get process count from command line arguments.
 *
 * @param argc		Arguments count
 * @param argv		Double char array containing command line arguments.
 *
 * @return -1 on error, any other values on success.
 */
size_t get_proc_count(int argc, char** argv){
	int proc_count;
	if (!strcmp(argv[1], "-p") && (proc_count = atoi(argv[2])) == (argc - 3)){
		return proc_count;
	}
	return -1;
}

/** Get process balance from command line arguments.
 *
 * @param proc_id	Process local id
 * @param argv		Double char array containing command line arguments.
 *
 * @return process balance
 */
balance_t get_proc_balance(local_id proc_id, char** argv){
	return atoi(argv[proc_id + 2);
}