#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(const int argc, const char* argv[])
{
	if( argc != 2 ) {
		errx(1, "One argument expected (got %d)", argc-1);
	}

	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) {
		err(1, "Failed to open file %s", argv[1]);
	}

	struct stat s;
	stat(argv[1], &s);
	size_t SZ = s.st_size/16;

	int arr[SZ];
	int totalLen = 0;
	for(size_t i=0; i<SZ; i++) {
		lseek(fd1, 8, SEEK_CUR);
		uint32_t time1;
		uint32_t time2;
		read(fd1, &time1, 4);
		read(fd1, &time2, 4);
		
		int time = (int)time2 - (int)time1;
		arr[i] = time;
		totalLen += time;
	}
	int avg = totalLen/SZ;
	int D = 0;
	for(size_t i=0; i<SZ; i++) {
		D += ((arr[i]-avg)*(arr[i]-avg))/SZ;
	}

	uint32_t users[2048];
	int j=0;
	lseek(fd1, 0, SEEK_SET);
	for(size_t i=0; i<SZ; i++) {
		uint32_t UID;
		uint16_t res1;
		uint16_t res2;
		uint32_t time1;
		uint32_t time2;
		read(fd1, &UID, 4);
		read(fd1, &res1, 2);
		read(fd1, &res2, 2);
		read(fd1, &time1, 4);
		read(fd1, &time2, 4);
		int time = (int)time2 - (int)time1;
		int sqTime = time*time;
		
		if( sqTime > D ) {
			int found = 0;
			for(int g=0; g<j; g++) {
				if( users[g] == UID ) { 
					found = 1;
					break;
				}
			}
			if( !found ) {
				users[j] = UID;
				j++;
			}
		}
	}

	for(int i=0; i<j; i++) {
		printf("UID: %x with longest session duration of ", users[i]);
		
		int maxSession = 0;
		lseek(fd1, 0, SEEK_SET);
		for(size_t k=0; k<SZ; k++) {
			uint32_t UID;
			read(fd1, &UID, 4);
			lseek(fd1, 4, SEEK_CUR);
			uint32_t time1;
			uint32_t time2;
			read(fd1, &time1, 4);
			read(fd1, &time2, 4);
			int time = (int)time2 - (int)time1;
			if( UID == users[i] ) {
				if( time > maxSession ) maxSession = time;
			}
		}

		printf("%d.\n", maxSession);
	}

	close(fd1);
	exit(0);
}
