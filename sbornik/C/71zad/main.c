#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(const int argc, const char* argv[])
{
	if( argc != 1 && argc != 2 ) {
		errx(1, "Format: ./main [<command>]");
	}

	char cmd[5];

	if( argc == 2 ) {
		strcpy(cmd, argv[1]);
	} else {
		strcpy(cmd, "echo");
	}

	char str[6];
	char m;
	int i = 0;
	while( read(0, &m, 1) ) {
		if( m!=0x20 && m!=0x0a ) {
			str[i] = m;
			i++;
			
			if( i == 5 ) {
				str[i] = '\0';
				errx(1, "String %s... is longer than 4 digits", str);
			}

		} else {
			str[i] = '\0';
			i = 0;

			pid_t a = fork();

			if( a == 0 ) {
				int ex;
				ex = execlp(cmd, cmd, str, 0);
				if( ex == -1 ) {
					errx(1, "Failed to execute command %s", cmd);
				}
			} else {
				wait(NULL);
				continue;
			}

		}
	}

	exit(0);
}
