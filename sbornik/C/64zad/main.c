#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(const int argc, const char* argv[])
{
	if( argc != 3 ) {
		errx(1, "Two arguments expected");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) {
		errx(1, "Failed to open file %s", argv[1]);
	}
	int fd2 = open(argv[2], O_RDONLY);
	if( fd2 == -1 ) {
		errx(1, "Failed to open file %s", argv[2]);
	}

	struct stat s;
	stat(argv[2], &s);
	size_t sz = s.st_size/2;
	
	int arr1[sz];
	int arr2[sz];

	uint8_t c;
	int j=0;
	while( read(fd1, &c, 1) ) {
		int arr[8];
		int i=7;
		while( c>0 ) {
			arr[i] = c%2;
			c /= 2;
			i--;
		}
		while( i>=0 ) {
			arr[i] = 0;
			i--;
		}

		for(int u=0; u<8; u++) {
			arr1[j] = arr[u];
			j++;
		}
	}

	uint16_t m;
	int l=0;
	while( read(fd2, &m, 2) ) {
		arr2[l] = m;
		l++;
	}

	int fd3 = open("out", O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if( fd3 == -1 ) {
		err(1, "Failed to create file out");
	}
	for(size_t i=0; i<sz; i++) {
		if( arr1[i] == 1 ) {
			write(fd3, &arr2[i], 2);
		}
	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
