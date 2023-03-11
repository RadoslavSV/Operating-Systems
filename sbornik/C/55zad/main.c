#include <stdio.h>
#include <stdint.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(const int argc, const char* argv[])
{
	if( argc != 4 ) {
		errx(1, "Three arguments expected");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) {
		err(1, "Failed to open file %s", argv[1]);
	}
	int fd2 = open(argv[2], O_RDONLY);
	if( fd2 == -1 ) {
		err(1, "Failed to open file %s", argv[2]);
	}
	int fd3 = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if( fd3 == -1 ) {
		err(1, "Failed to open file %s", argv[3]);
	}

	struct stat s;
	stat(argv[2], &s);
	int SZ = s.st_size;

	uint8_t arr[SZ];
	for(int i=0; i<SZ; i++) {
		uint8_t c;
		read(fd2, &c, 1);
		arr[i] = c;
	}


	struct stat t;
	stat(argv[1], &t);
	int pSZ = t.st_size/4;
	
	for(int i=0; i<pSZ; i++) {
		uint16_t offset;
		uint8_t original;
		uint8_t new;
		read(fd1, &offset, 2);
		read(fd1, &original, 1);
		read(fd1, &new, 1);

		if( offset > SZ ) {
			errx(1, "Such offset does not exist");
		}

		if( arr[offset] == original ) {
			arr[offset] = new;
		} else {
			errx(1, "Original byte is not the same as supposed to be");
		}
	}

	for(int i=0; i<SZ; i++) {
		write(fd3, &arr[i], 1);
	}



	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
