#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void)
{
	int x[2];
   	int y[2];
	pipe(x);
	pipe(y);

	pid_t a = fork();

	if( a == 0 ) {
		//first
		close(y[0]);
		close(y[1]);

		close(x[0]);
		dup2(x[1], 1);
		execlp("cut", "cut", "-d", ":", "-f", "7", "/etc/passwd", 0);
	} else {
		pid_t b = fork();

		if( b == 0 ) {
			//second
			close(x[1]);
			dup2(x[0], 0);

			close(y[0]);
			dup2(y[1], 1);
			execlp("sort", "sort", 0);
		} else {
			//third
			close(x[0]);
			close(x[1]);

			close(y[1]);
			dup2(y[0], 0);
			execlp("uniq", "uniq", "-c", 0);
		}
	}

	exit(0);
}
