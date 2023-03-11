#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>

int main(const int argc, const char* argv[])
{
	if( argc != 2 ) {
		errx(1, "One argument is expected");
	}

	int fd1 = open(argv[1], O_RDWR);
	if( fd1 == -1 ) {
		err(1, "Failed to open file %s", argv[1]);
	}

	uint64_t arr[256];
	for(int i=0; i<256; i++) {
		arr[i] = 0;
	}

	uint8_t c;
	int rd;
	while( (rd = read(fd1, &c, 1)) ) {
		arr[c]++;
	}
	if( rd == -1 ) {
		err(1, "Failed to read file %s", argv[1]);
	}

	lseek(fd1, 0, SEEK_SET);

	for(int i=0; i<256; i++) {
		//printf("%hhx - %ld\n", i, arr[i]);
		while( arr[i]>0 ) {
			write(fd1, &i, 1);
			arr[i]--;
		}
	}

	close(fd1);

	exit(0);
}
