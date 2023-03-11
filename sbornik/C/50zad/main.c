#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

int main(void)
{
	int fd1 = open("f1", O_RDONLY);
	if( fd1 == -1 ) {
		err(1, "Failed to open file f1");
	}
	
	int fd2 = open("f2", O_RDONLY);
	if( fd2 == -1 ) {
		err(1, "Failed to open file f2");
	}

	uint32_t b;
	size_t n = 0;
	while( read(fd1, &b, sizeof(b)) ) {
		n++;
	}
	n /= 2;
	lseek(fd1, 0, SEEK_SET);

	uint32_t pos[n];
	uint32_t len[n];

	for(size_t i=0; i<n; i++) {
		read(fd1, &b, sizeof(b));
		pos[i] = b;
		read(fd1, &b, sizeof(b));
		len[i] = b;
	}

	//for(size_t i=0; i<n; i++) { printf("%d - %d\n", pos[i], len[i]); }

	int fd3 = open("f3", O_CREAT|O_TRUNC|O_WRONLY, 0644);

	for(size_t i=0; i<n; i++) {
		lseek(fd2, 0, SEEK_SET);
		while(pos[i]>0) { 
			read(fd2, &b, sizeof(b));
			pos[i]--;
		}

		for(size_t j=0; j<len[i]; j++) {
			ssize_t read_size;
			read_size = read(fd2, &b, sizeof(b));
			write(fd3, &b, read_size);
		}
	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
