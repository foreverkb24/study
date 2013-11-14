#include <stdio.h>
#include <stdlib.h>

int main() {

	int process1 = fork();
	if(process1 < 0) {
		printf("fork failed!\n");
		exit(1);
	}
	else if(process1 == 0) {
		printf("child process1:B\n");
	}
	else {
		printf("father process:A\n ");
        int	process1 = fork();
		if(process1 < 0) {
			printf("fork failed!\n");
			exit(1);
		}
		else if(process1 == 0) {
			printf("child process1:C\n");
		}
		else {
		} 
		exit(0);
	}
}
