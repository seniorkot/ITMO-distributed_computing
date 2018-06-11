/**
 * @file     log4pa.c
 * @Author   @seniorkot
 * @date     May, 2018
 * @brief    Logger functions
 */
 
#include "log4pa.h"
#include "common.h"
#include "pa2345.h"
#include "lamport.h"

#include <stdio.h>
#include <unistd.h>

FILE* pipes_log_f;
FILE* events_log_f;

void log_init(){
	pipes_log_f = fopen(pipes_log, "w");
	events_log_f = fopen(events_log, "w");
}

void log_destroy(){
	fclose(pipes_log_f);
    fclose(events_log_f);
}

void log_pipes(PipesCommunication* comm){
	size_t i;
	
	fprintf(pipes_log_f, "Process %d pipes:\n", comm->current_id);
	
	for (i = 0; i < comm->total_ids; i++){
		if (i == comm->current_id){
			continue;
		}
		
		fprintf(pipes_log_f, "P%ld|R%d|W%d ", i, comm->pipes[(i < comm->current_id ? i : i-1) * 2 + PIPE_READ_TYPE], 
			comm->pipes[(i < comm->current_id ? i : i-1) * 2 + PIPE_WRITE_TYPE]);
	}
	fprintf(pipes_log_f, "\n");
}

void log_started(local_id id, balance_t balance){
	printf(log_started_fmt, get_lamport_time(), id, getpid(), getppid(), 0);
    fprintf(events_log_f, log_started_fmt, get_lamport_time(), id, getpid(), getppid(), 0);
}

void log_received_all_started(local_id id){
	printf(log_received_all_started_fmt, get_lamport_time(), id);
    fprintf(events_log_f, log_received_all_started_fmt, get_lamport_time(), id);
}

void log_done(local_id id, balance_t balance){
	printf(log_done_fmt, get_lamport_time(), id, 0);
    fprintf(events_log_f, log_done_fmt, get_lamport_time(), id, 0);
}

void log_received_all_done(local_id id){
	printf(log_received_all_done_fmt, get_lamport_time(), id);
    fprintf(events_log_f, log_received_all_done_fmt, get_lamport_time(), id);
}
