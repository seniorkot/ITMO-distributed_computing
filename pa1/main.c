/**
 * @file     main.c
 * @Author   Oleg Ivanko (@seniorkot) & Martin Rayla
 * @date     May, 2018
 */
 
#include <string.h>
#include <sys/types>
#include <unistd.h>

/** Get process count from command line arguments.
 *
 * @param argv		Double char array containing command line arguments.
 *
 * @return -1 on error, any other values on success.
 */
int get_proc_count(char** argv);

int main(int argc, char** argv){
	return 0;
}

int get_proc_count(char** argv){
	if (strcmp(argv[1], "-p")){
		return atoi(argv[2]);
	}
	return -1;
}