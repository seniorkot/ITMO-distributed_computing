/**
 * @file     communication.c
 * @Author   @seniorkot
 * @date     May, 2018
 * @brief    Functions that help to organize IPC
 */
 
#include "communication.h"

/** Open pipes fds
 * 
 * @param proc_count    Process count including parent process.
 *
 * @return pointer to pipe fds array
 */
int* pipes_init(size_t proc_count){
	size_t i, j;
	size_t offset = proc_count - 1;
	int* pipes = malloc(sizeof(int) * 2 * proc_count * (proc_count-1));
	
	for (i = 0; i < proc_count; i++){
		for (j = 0; j < proc_count; j++){
			int tmp_fd[2];
			
			if (i == j){
				continue;
			}
			
			if (pipe(tmp_fd) < 0){
				return (int*)NULL;
			}
			pipes[i * offset * 2 + (i > j ? j : j - 1) * 2 + PIPE_READ_TYPE] = tmp_fd[0];  /* READ */
			pipes[j * offset * 2 + (j > i ? i : i - 1) * 2 + PIPE_WRITE_TYPE] = tmp_fd[1]; /* WRITE */
		}
	}
	return pipes;
}

/** Init PipesCommunication
 * 
 * @param pipes			Pointer to opened pipes fd
 * @param proc_count    Process count including parent process.
 * @param curr_proc		Current process local id
 *
 * @return pointer to PipesCommunication
 */
PipesCommunication* communication_init(int* pipes, size_t proc_count, local_id curr_proc){
	PipesCommunication* this = malloc(sizeof(PipesCommunication));;
	size_t i, j;
	size_t offset = proc_count - 1;
	this->pipes = malloc(sizeof(int) * offset * 2);
	this->total_ids = proc_count;
	this->current_id = curr_proc;
	
	memcpy(this->pipes, pipes + curr_proc * 2 * offset, sizeof(int) * offset * 2);
	
	/* Close unnecessary fds */
	for (i = 0; i < proc_count; i++){
		if (i == curr_proc){
			continue;
		}
		for (j = 0; j < proc_count; j++){
			close(pipes[i * offset * 2 + (i > j ? j : j - 1) * 2 + PIPE_READ_TYPE]);
			close(pipes[i * offset * 2 + (i > j ? j : j - 1) * 2 + PIPE_WRITE_TYPE]);
		}
	}
	free(pipes);
	return this;
}

/** Close all fds & free space
 * 
 * @param comm		Pointer to PipesCommunication
 */
void communication_destroy(PipesCommunication* comm){
	size_t i;
	for (i = 0; i < comm->total_ids - 1; i++){
		close(comm->pipes[i * 2 + PIPE_READ_TYPE]);
		close(comm->pipes[i * 2 + PIPE_WRITE_TYPE]);
	}
	free(comm);
}
