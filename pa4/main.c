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
	
}

/** Do child process work
 *
 * @param comm		Pointer to PipesCommunication
 * @param mutexl	Mutexl flag
 *
 * @return -1 on error, 0 on success.
 */
int do_child_work(PipesCommunication* comm, int mutexl){
	
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
		else if (res == '?'}{
			return -1;
		}
	}
	return 0;
}
