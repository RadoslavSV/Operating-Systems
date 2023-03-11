#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>

void Sort(const char* file);

void Sort(const char* file) {
	int fd1 = open(file, O_RDWR);
	if( fd1 == -1 ) {
		err(1, "Failed to open file %s", file);
	}

	struct stat s1;
	stat(file, &s1);
	int Sz = s1.st_size/4;

	uint32_t arr[Sz];
	uint32_t n;
	int u = 0;
	while( read(fd1, &n, 4) ) {
		arr[u] = n;
		u++;
	}

	for(int i=0; i<Sz; i++) {
		for(int j=i; j<Sz; j++) {
			if( arr[i] > arr[j] ) {
				uint32_t tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}

	lseek(fd1, 0, SEEK_SET);
	for(int i=0; i<Sz; i++) {
		write(fd1, &arr[i], 4);
	}

	close(fd1);
}

uint32_t retrieve(int fd, int i);

uint32_t retrieve(int fd, int i) {
	lseek(fd, i * sizeof(uint32_t), SEEK_SET);
	uint32_t N;
	read(fd, &N, 4);
	return N;
}

int main(const int argc, char* argv[])
{
	if( argc != 2 ) {
		errx(1, "One argument expected");
	}

	int fd = open(argv[1], O_RDWR);
	int fd1 = open("FILE1", O_CREAT|O_TRUNC|O_RDWR, 0644);
	int fd2 = open("FILE2", O_CREAT|O_TRUNC|O_RDWR, 0644);
	if( fd == -1 || fd1 == -1 || fd2 == -1 ) err(1, "File descriptors error");

	struct stat s1;
	stat(argv[1], &s1);
	int sz = s1.st_size/4;

	uint32_t m;
	for(int i=0; i<sz/2; i++) {
		read(fd, &m, 4);
		write(fd1, &m, 4);
	}
	while( read(fd, &m, 4) ) {
		write(fd2, &m, 4);
	}

	Sort("FILE1");
	Sort("FILE2");

	lseek(fd, 0, SEEK_SET);
	lseek(fd1, 0, SEEK_SET);
	lseek(fd2, 0, SEEK_SET);

	//printf("%x\n", retrieve(fd2, 1));

	struct stat S1, S2;
	stat("FILE1", &S1);
	stat("FILE2", &S2);

	for(int i=0, j=0; i<S1.st_size/4 && j<S2.st_size/4; ) {
		uint32_t a, b;
		a = retrieve(fd1, i);
		b = retrieve(fd2, j);
		if( a < b && i<S1.st_size/4) {
			write(fd, &a, 4);
			i++;
		} else if( b <= a && j<S2.st_size/4 ) {
			write(fd, &b, 4);
			j++;
		}
		if( i==S1.st_size/4 ) {
			while( j<S2.st_size/4 ) {
				b = retrieve(fd2, j);
				write(fd, &b, 4);
				j++;
			}
		}
		if( j==S2.st_size/4 ) {
			while( i<S1.st_size/4 ) {
				a = retrieve(fd1, i);
				write(fd, &a, 4);
				i++;
			}
		}
	}

	remove("FILE1");
	remove("FILE2");
	close(fd);
	close(fd1);
	close(fd2);
	exit(0);
}
