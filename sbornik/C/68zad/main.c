#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(const int argc, const char* argv[])
{
	if( argc != 2 ) {
		errx(1, "1 argument expected");
	}

	int x[2];
	if( pipe(x) == -1 ) {
		errx(1, "Failed to pipe");
	}

	pid_t a = fork();
	if( a == -1 ) {
		errx(1, "Failed to fork");
	}

	if( a == 0 ) {
		//first

		close(x[0]);
		dup2(x[1], 1);
		execlp("cat", "cat", argv[1], 0);
	} else {
		//second
		wait(NULL);

		close(x[1]);
		dup2(x[0], 0);
		execlp("sort", "sort", 0);
	}

	exit(0);
}
