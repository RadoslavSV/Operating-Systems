#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <sys/wait.h>

int main(const int argc, const char* argv[])
{
	if( argc != 3 ) {
		errx(1, "Two arguments expected");
	}
	
	int N = atoi(argv[1]);
	int D = atoi(argv[2]);
	
	pid_t a = fork();
	if( a == -1 ) {
		err(1, "Failed to fork");
	}

	if( a == 0 ) {
		//CHILD
		for(int i=0; i<N; i++) {
			wait(NULL);
			write(1, "DONG\n", 5);
		}
	} else {
		//PARENT
		for(int i=0; i<N; i++) {
			write(1, "DING\n", 5);
			wait(NULL);
			sleep(D);
		}
	}



	exit(0);
}
