#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(const int argc, const char* argv[])
{
	if( argc == 1 ) {
		
		char c;
		char file[1000];
		int i = 0;
		while( read(0, &c, sizeof(c)) ) {
			if( c != '\n' ) {
				file[i] = c;
				i++;
			} else {
				file[i] = '\0';
				i = 0;
				
				//printf("file is %s\n", file);
				int fd1 = open(file, O_RDONLY);
				if( fd1 == -1 ) {
					warn("Failed to open file %s", file);
				} else {
					char m;
					while( read(fd1, &m, 1) ) {
						write(1, &m, 1);
					}
				}
				close(fd1);
			}
		}

	} else {
	
		for(int i=1; i<argc; i++) {
			if( strcmp(argv[i], "-") == 0 ) {
				
				char c;
				char file[1000];
				int j = 0;
				while( read(0, &c, sizeof(c)) ) {
					if( c != '\n' ) {
						file[j] = c;
						j++;
					} else {
						file[j] = '\0';
						j = 0;
				
						int fd1 = open(file, O_RDONLY);
						if( fd1 == -1 ) {
							warn("Failed to open file %s", file);
						} else {
							char m;
							while( read(fd1, &m, 1) ) {
								write(1, &m, 1);
							}
						}
						close(fd1);
					}
				}	
				
			} else {
				
				int fd1 = open(argv[i], O_RDONLY);
				if( fd1 == -1 ) {
					warn("Failed to open file %s", argv[i]);
				} else {
					char p;
					while( read(fd1, &p, 1) ) {
						write(1, &p, 1);
					}
				}
				close(fd1);
			}
		}	

	}


	exit(0);
}
