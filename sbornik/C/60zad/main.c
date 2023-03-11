#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(const int argc, const char* argv[])
{
	if( argc != 3 ) {
		errx(1, "Two arguments expected");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) {
		err(1, "Failed to open file %s", argv[1]);
	}

	struct stat s;
	stat(argv[1], &s);
	int SZ = s.st_size/2;
	int arr[SZ];

	uint16_t c;
	int k=0;
	while( read(fd1, &c, 2) ) {
		arr[k] = c;
		k++;
	}

	for(int i=0; i<SZ; i++) {
		for(int j=0; j<SZ-i-1; j++) {
			if( arr[j] > arr[j+1] ) {
				int tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}

	int fd2 = open(argv[2], O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if( fd2 == -1 ) {
		err(1, "Failed to create file %s", argv[2]);
	}

	for(int i=0; i<SZ; i++) {
		write(fd2, &arr[i], 2);
	}

	close(fd1);
	close(fd2);
	exit(0);
}
