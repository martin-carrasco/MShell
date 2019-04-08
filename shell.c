#include "shell_functs.c"
#include <sys/wait.h>
#include <signal.h>

#define TOKEN_BUFFER_SIZE 32
#define READ_BUFFER_SIZE 128

//Returns a byte containing flags for the type of command to be executed
//b0 - Whether an allowed command or not
//b1 - Wether it's an execv command or my own implmentation
//b2 -  Void
//b3 - Void
char eval_command(char* cmd){
	char opts = 0;
	for(int x = 0;x < num_allowed_commands();x++){
		if(strcmp(cmd, allowed_commands[x]) == 0)
			opts |= 0b1;	
	}
	for(int x = 0;x < num_exec_commands();x++){
		if(strcmp(cmd, exec_commands[x]) == 0)
			opts |= 0b10;
	}
	return opts;
}

//Read lines from the console input into the buffer
char* read_line(void){
	int pos = 0;
	char* console_buffer = malloc(READ_BUFFER_SIZE*sizeof(char));
	
	if(console_buffer == NULL)
		perror("Memory allocation failed");
	while(1){
		char c;
		c = getchar();

		if(c == EOF || c == '\n'){
			console_buffer[pos] = '\0';
			break;
		}
		if(pos < READ_BUFFER_SIZE)
			console_buffer[pos++] = c;	
		
		else
			perror("Buffer limit exceeded");

	}

	return console_buffer;
}
//Splits lines based on tokens and returns it
char** split_line(char* line){
	int pos = 0;
	char** argc = malloc(sizeof(char*) * TOKEN_BUFFER_SIZE);
	char* token;
	char delimiter[] = " ";
	token = strtok(line, delimiter);
	
	if(token == NULL){
		perror("Empty delimiter");
	}

	while(token != NULL){
		argc[pos++] = token;
		token = strtok(NULL, delimiter);
		if(pos > TOKEN_BUFFER_SIZE-1)
			perror("Token buffer size exceeded");
	}

	return argc;
}

//Creating of a new process and handling
int exec(char** args){
	pid_t pid;
	int status;
	
	pid = fork();
	if(pid== 0){
		char opts = eval_command(args[0]);
		if(opts & 0b1 == 1){
			if( (opts >>  1) == 1){
				if(execvp(args[0], args) == -1)
					perror("Error executing");
			}
			else{ 
				getFunct(args[0])(args);
			}

		}
		else
			printf("Command not recognized\n");

	}
	else if(pid < 0){
		perror("error");
		exit(1);
	}
	else{
		do{
			waitpid(pid, &status, WUNTRACED);
		}while(!WIFSIGNALED(status) && !WIFEXITED(status));
	}
	return 1;
}
//Main loop for handling console input
void main_loop(void){
	while(1){
		prompt_print();
		char* inpt = read_line();
		if(inpt == NULL || *inpt == '\0'){
			continue;
		}

		char** args = split_line(inpt);
		int run = exec(args);
		free(inpt);
		free(args);
	}
}

int main(){
	prompt_print();
	write(1, "\33[H\33[2J", 7); // Fills the screen with empty spaces (like clear)
	main_loop();
	
	return 0;
}
