#include <unistd.h>
#include <signal.h> // signal is included here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER 80
static const char*historyArray[BUFFER];
static int historyCount = 0;

void sigint_handler(int sig) {
	write(1,"Terminating through signal handler\n",35); 
	exit(0);
}

void helpPrint() {
	printf("Siyu's mini-shell\n");
	printf("Built-in Commands are:\n");
	printf("cd\n");
	printf("exit\n");
	printf("history\n");
	printf("help\n");
}
void historyFunction(char* input) {
	historyArray[historyCount++] = strdup(input);	
}

int main(){
	alarm(180); // Please leave in this line as the first statement in your program.
              // This will terminate your shell after 180 seconds,
              // and is useful in the case that you accidently create a 'fork bomb'
	
	signal(SIGINT, sigint_handler); // terminate by pressing Ctrl+C
	
	char input[BUFFER]; // holds 80 characters at most
	while(1){
		// runs forever
		printf("mini-shell>");
		fgets(input, BUFFER, stdin); // getting user input
		input[strcspn(input, "\n")] = '\0';  // replace the \n with \0 when user press enter
		historyFunction(input);
		// parse the arguments here for the child to run
		char* array[BUFFER];
		char* token = strtok(input, " ");
		int i = 0;
		while(token != NULL) {
			array[i++] = strdup(token);
			token = strtok(NULL, " ");
		}
		array[i] = NULL;
		// have bulit-in program:
		// cd function
		if(strcmp(array[0],"cd")==0){
			chdir(array[1]);
		}
		// history
		else if(strcmp(array[0], "history")==0){
			for(int j=0; j<historyCount; j++) {
				printf("%d %s\n",j+1, historyArray[j]);
			}		
		}
		// exit
		else if(strcmp(array[0],"exit")==0){
			exit(0);
		}
		// help 
		else if(strcmp(array[0],"help")==0){
			helpPrint();
		}else{
			//child process responsible for the terminal commands
			int child_status;
			if (fork() == 0){	
				execvp(array[0], array);
				printf("Commands not found! Enter antoher command\n");
				exit(1);
			}else{
				//parent wait till child finishes continue the while loop
				wait(&child_status);	
			}
		}		
	}

	return 0;
}
