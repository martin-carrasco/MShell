#include "vars.h"
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

void ls_func(char** args);
void cd_func(char** args);
void rm_func(char** args);
void pwd_func(char** args);
void clear_func(char** args);

void prompt_print(void);
int num_allowed_commands(void);
int num_exec_commands(void);

void (*allowed_commands_ptr[]) (char** argc) = {
	&ls_func,
	&rm_func,
	&cd_func,
	&pwd_func,
	&clear_func
};

void clear_func(char** args){
	prompt_print();
	write(1, "\33[H\33[2J", 7);
}
void ls_func(char** args){ //TODO Add safeguards for using chdir
	char* current_dir = malloc(sizeof(char)*128);
	getcwd(current_dir, 128);
	
	chdir(args[1]);
	char* dir_name = malloc(sizeof(char)*64);
	getcwd(dir_name, 64);

	DIR* dir_stream = opendir(dir_name);
	if(dir_stream == NULL){
		printf("Error opening current dir\n");	
		return;
	}
	struct dirent* f_stream = readdir(dir_stream);
	while((f_stream = readdir(dir_stream)) != NULL){	
		printf("%s\t", f_stream->d_name);
		fflush(stdout);
	}
	printf("\n");
	closedir(dir_stream);
	chdir(current_dir);
}
void cd_func(char** args){
	int res = chdir(args[1]);
	if(res == 0)
		return;
	int errsv = errno;
	switch(errno)
	{
		case ENOTDIR:
			printf("Not a directory\n");
			break;
		case ENOENT:
			printf("Not a valid path\n");
			break;
		default:
			printf("Invalid arguments\n");
			break;
	}
}
void rm_func(char** args){

}
void pwd_func(char** args){
}

fnct_ptr_type getFunct(char* cmd){
	for(int x = 0;x < num_allowed_commands();x++){
		if(strcmp(cmd, allowed_commands[x]) == 0)
			return allowed_commands_ptr[x];
	}
	return NULL;
}

int num_allowed_commands(){
	return (sizeof(allowed_commands) / sizeof(allowed_commands[0]));
}
int num_exec_commands(){
	return (sizeof(exec_commands) / sizeof(exec_commands[0]));
}

void prompt_print(){
	char* buffer = malloc(sizeof(char)*128);
	getcwd(buffer, 128);
	printf("%sMartin%s::%s%s%s$%s ", COLOR_RED, COLOR_GREEN, COLOR_CYAN, buffer, COLOR_GREEN, COLOR_RESET);
       	//Format %s%s(Color and name) %s%s (Color and Separator1) %s%s(Color and Dir)  %s%s (Color and separator2) %s(ColorReset)
	fflush(stdout);
}
