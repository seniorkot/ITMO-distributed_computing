/**
 * @file     main.c
 * @Author   Oleg Ivanko (@seniorkot) & Martin Rayla
 * @date     May, 2018
 */
 
#include <string.h>
#include <sys/types>
#include <unistd.h>

#define PARENT_ID 0

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
	
	/* Resolving program arguments */
	switch(argc){
		case 1:
			proc_count = 0; /* TODO: set new default value */
			break;
		case 3:
			proc_count = get_proc_count(argv);
			if (proc_count == -1){
				fprintf(stderr, "Usage: %s -p X", argv[0]);
				return -1;
			}
			break;
		default:
			fprintf(stderr, "Usage: %s -p X", argv[0]);
			return -1;
	}
	
	/* Allocate memory for children */
	children = malloc(sizeof(pid_t) * proc_count);
	
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
	
	if (!fork_id){
		current_proc_id = i + 1;
	}
	else{
		current_proc_id = PARENT_ID;
	}
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