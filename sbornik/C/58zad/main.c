#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

int main(const int argc, const char* argv[])
{
	if( argc != 3 ) errx(1, "Two arguments expected");

	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) err(1, "Failed to open %s", argv[1]);
	int fd2 = open(argv[2], O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if( fd2 == -1 ) err(1, "Fail with %s", argv[2]);

	struct stat s;
	stat(argv[1], &s);
	int sz = s.st_size/4;

	int arr1Sz = 0, arr2Sz = 0;
	if( sz%2!=0 ) {
		arr1Sz = sz/2;
		arr2Sz = sz/2+1;
	} else {
		arr1Sz = sz/2;
		arr2Sz = sz/2;
	}
	uint32_t arr1[arr1Sz], arr2[arr2Sz];

	uint32_t m;
	for(int i=0; i<arr1Sz; i++) {
		read(fd1, &m, 4);
		arr1[i] = m;
	}
	int u=0;
	while( read(fd1, &m, 4) ) {
		arr2[u] = m;
		u++;
	}

	for(int i=0; i<arr1Sz; i++) {
		for(int j=0; j<arr1Sz-i-1; j++) {
			if( arr1[j]>arr1[j+1] ) {
				uint32_t tmp = arr1[j];
				arr1[j] = arr1[j+1];
				arr1[j+1] = tmp;
			}
		}
	}
	for(int i=0; i<arr2Sz; i++) {
		for(int j=0; j<arr2Sz-i-1; j++) {
			if( arr2[j]>arr2[j+1] ) {
				uint32_t tmp = arr2[j];
				arr2[j] = arr2[j+1];
				arr2[j+1] = tmp;
			}
		}
	}

	/*
	for(int i=0; i<arr1Sz; i++) {
		printf("%d ", arr1[i]);
	} printf("\n");
	for(int i=0; i<arr2Sz; i++) {
		printf("%d ", arr2[i]);
	} printf("\n");
	*/

	for(int i=0, j=0; i<arr1Sz && j<arr2Sz; ) {
		if( arr1[i] < arr2[j] ) {
			write(fd2, &arr1[i], 4);
			i++;
		} else {
			write(fd2, &arr2[j], 4);
			j++;
		}
		if( i==arr1Sz ) {
			while( j<arr2Sz ) {
				write(fd2, &arr2[j], 4);
				j++;
			}
		}
		if( j==arr2Sz ) {
			while( i<arr1Sz ) {
				write(fd2, &arr1[i], 4);
				i++;
			}
		}
	}	

	
	close(fd1);
	close(fd2);
	exit(0);
}
