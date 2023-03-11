#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>

int main(const int argc, const char* argv[])
{
	if( argc != 4 ) {
		errx(1, "3 arguments expected");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) {
		err(1, "Failed to open file %s", argv[1]);
	}
	int fd2 = open(argv[2], O_RDONLY);
	if( fd2 == -1 ) {
		err(1, "Failed to open file %s", argv[2]);
	}
	
	struct stat s1;
	stat(argv[1], &s1);
	//printf("%s size is %lu bytes\n", argv[1], s1.st_size);
	
	struct stat s2;
	stat("f2.bin", &s2);
	//printf("%s size is %lu bytes\n", argv[2], s2.st_size);
	
	if( s1.st_size != s2.st_size ) {
		errx(1, "%s and %s do not have equal sizes", argv[1], argv[2]);
	}

	uint8_t b1;
	uint8_t b2;
	uint16_t d = 0;

	int fd = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if( fd == -1 ) {
		err(1, "Failed to open file %s", argv[3]);
	}

	while( read(fd1, &b1, sizeof(b1)) ) {
		while( read(fd2, &b2, sizeof(b2)) ) {
			//printf("%hhx: %hhx VS %hhx\n", d, b1, b2);
			
			if( b1!=b2 ) {
				write(fd, &d, sizeof(d));
				write(fd, &b1, sizeof(b1));
				write(fd, &b2, sizeof(b2));
			}

			break;
		}
		d++;
	}

	close(fd1);
	close(fd2);
	close(fd);
	exit(0);
}
