/**
 * @file     log1pa.c
 * @Author   Oleg Ivanko (@seniorkot) & Martin Rayla
 * @date     May, 2018
 * @brief    Logger functions
 */
 
#include "log1pa.h"

void log_init(){
	pipes_log_f = fopen(pipes_log, "w");
	events_log_f = fopen(events_log, "w");
}

void log_started(local_id id){
	printf(log_started_fmt, id, getpid(), getppid());
    fprintf(events_log_f, log_started_fmt, id, getpid(), getppid());
}

void log_received_all_started(local_id id){
	printf(log_received_all_started_fmt, id);
    fprintf(events_log_f, log_received_all_started_fmt, id);
}

void log_done(local_id id){
	printf(log_done_fmt, id);
    fprintf(events_log_f, log_done_fmt, id);
}

void log_received_all_done(local_id id){
	printf(log_received_all_done_fmt, id);
    fprintf(events_log_f, log_received_all_done_fmt, id);
}
void log_destroy(){
	fclose(pipes_log_f);
    fclose(events_log_f);
}
