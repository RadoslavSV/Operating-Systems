#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>

int main(const int argc, const char* argv[])
{
	if( argc != 2 ) {
		errx(1, "One arguments (of type directory) is expected");
	}

	int x[2];
	pipe(x);
	pid_t a = fork();

	if( a==0 ) {
		//FIRST
		close(x[0]);
		dup2(x[1], 1);
		execlp("find", "find", argv[1], "-type", "f", "-printf", "%P_%T@\n", 0);
	} else {
		close(x[1]);

		int y[2];
		pipe(y);

		pid_t b = fork();

		if( b==0 ) {
			//SECOND
			close(y[0]);
			dup2(y[1], 1);
			dup2(x[0], 0);
			execlp("sort", "sort", "-t", "_", "-k", "2,2nr", 0);
		} else {
			close(x[0]);
			close(y[1]);

			int z[2];
			pipe(z);

			pid_t c = fork();

			if( c==0 ) {
				//THIRD
				close(z[0]);
				dup2(z[1], 1);
				dup2(y[0], 0);
				execlp("head", "head", "-n", "1", 0);
			} else {
				//FOURTH
				close(y[0]);
				close(z[1]);
				dup2(z[0], 0);
				execlp("cut", "cut", "-d", "_", "-f", "1", 0);
			}
		}
	}

	exit(0);
}
