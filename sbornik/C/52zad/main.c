#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

int main(const int argc, const char* argv[])
{
	if( argc != 5 ) {
		errx(1, "4 arguments expected");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) {
		err(1, "Failed to open file %s", argv[1]);
	}
	int fd2 = open(argv[2], O_RDONLY);
	if( fd2 == -1 ) {
		err(1, "Failed to open file %s", argv[2]);
	}
	int fd3 = open(argv[3], O_CREAT|O_TRUNC|O_RDWR, 0644);
	if( fd3 == -1 ) {
		err(1, "Failed to open file %s", argv[3]);
	}
	int fd4 = open(argv[4], O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if( fd4 == -1 ) {
		err(1, "Failed to open file %s", argv[4]);
	}
	
	uint16_t d2, maxD = 65535;
	uint8_t p2, p1;
	uint8_t e2;
	
	while( read(fd2, &d2, sizeof(d2)) ) {
		read(fd2, &p2, sizeof(p2));
		read(fd2, &e2, sizeof(e2));
		
		lseek(fd1, d2, SEEK_SET);
		read(fd1, &p1, sizeof(p1));
		
		if( p1<'A' || p1>'Z' ) continue;

		lseek(fd1, -1, SEEK_CUR);
		for(int i=0; i<p2; i++) {
			read(fd1, &p1, sizeof(p1));
			write(fd3, &p1, sizeof(p1));
		}

		if( maxD < d2) break;
		maxD = d2;
	}
	lseek(fd3, 0, SEEK_SET);


	uint8_t r;
	uint16_t d3 = 0;
	uint8_t p3 = 0;
	uint8_t e3 = 0;
	while( read(fd3, &r, sizeof(r)) ) {
		if( r>'A' && r<'Z' ) {
			write(fd4, &d3, sizeof(d3));
			p3 = 1;

			while( read(fd3, &r, sizeof(r)) ) {
				if( r<'A' || r>'Z' ) {
					p3++;
					d3++;
				}
				else 
				{	
					lseek(fd3, -1, SEEK_CUR);
					break;
				}
			}
			
			write(fd4, &p3, sizeof(p3));
			write(fd4, &e3, sizeof(e3));
		}
		d3++;
	}

	
	
	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
	exit(0);
}
