#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(const int argc, const char* argv[])
{
	if( argc != 2 ) {
		errx(1, "One argument expected");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) {
		err(1, "Failed to open file %s", argv[1]);
	}

	struct stat s;
	stat(argv[1], &s);
	int sz = s.st_size/16;

	pid_t pids[sz];
	int x[2];
	pipe(x);

	uint64_t filename;
	uint32_t offset;
	uint32_t length;
	for(int cnt=0; cnt<sz; cnt++) {
	
		pids[cnt] = fork();

		if( pids[cnt] == 0 ) {
			//CHILD

			read(fd1, &filename, 8);
			read(fd1, &offset, 4);
			read(fd1, &length, 4);
			char name = (char)filename;
			char file[2];
			file[0] = name;
			file[1] = '\0';

			int fd2 = open(file, O_RDONLY);
			if( fd2 == 1 ) {
				err(1, "Failed to open file %s", file);
			}
		
			lseek(fd2, offset*2, SEEK_SET);
	
			uint16_t arr[length];
			for(uint32_t i=0; i<length; i++) {
				uint16_t m;
				read(fd2, &m, 2);
				arr[i] = m;
			}

			uint16_t res;
			if( length > 0 ) {
				res = arr[0];
			} else {
				res = 0;
			}
			for(uint32_t i=1; i<length; i++) {
				res = res^arr[i];
			}
		
			//printf("%s: res = %d\n", file, res);
			
			close(x[0]);
			write(x[1], &res, 2);

			close(fd2);
			exit(0);
		}
	}

	//PARENT
	uint16_t final = 0;
	for(int cnt=0; cnt<sz; cnt++) {
		wait(NULL);
		close(x[1]);
		uint16_t r;
		read(x[0], &r, 2);
		if( cnt == 0 ) {
			final = r;
		} else {
			final = final^r;
		}
	}
	printf("%x\n", final);

	close(fd1);
	exit(0);
}
