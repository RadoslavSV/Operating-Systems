#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(const int argc, const char* argv[])
{
	if( argc != 3 ) {
		errx(1, "Exactly two arguments expected");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) {
		err(1, "Failed to open file %s", argv[1]);
	}

	int fd2 = open(argv[2], O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if( fd2 == -1 ) {
		err(1, "Failed to create/open file %s", argv[2]);
	}

	struct stat s;
	stat(argv[1], &s);
	int szX = s.st_size;

	for(int i=0; i<szX; i++) {
		uint8_t c;
		read(fd1, &c, 1);
		int arr[8];
		int j=7;
		while( c>0 ) {
			arr[j] = c%2;
			c /= 2;
			j--;
		}
		while( j>=0 ) {
			arr[j] = 0;
			j--;
		}

		int arr1[8], arr2[8], k=0, i1=0, i2=0;
		for( ; k<4; k++, i1+=2) {
			if( arr[k] == 0 ) {
				arr1[i1] = 0;
				arr1[i1+1] = 1;
			} else {
				arr1[i1] = 1;
				arr1[i1+1] = 0;
			}
		}
		for( ; k<8; k++, i2+=2) {
			if( arr[k] == 0 ) {
				arr2[i2] = 0;
				arr2[i2+1] = 1;
			} else {
				arr2[i2] = 1;
				arr2[i2+1] = 0;
			}
		}

		uint8_t num = 0, num2 = 0;
		int two = 1;
		for(int h=7; h>=0; h--, two*=2) {
			num += arr1[h]*two;
		}
		two = 1;
		for(int h=7; h>=0; h--, two*=2) {
			num2 += arr2[h]*two;
		}

		write(fd2, &num, sizeof(num));
		write(fd2, &num2, sizeof(num2));
	}

	close(fd1);
	close(fd2);
	exit(0);
}
