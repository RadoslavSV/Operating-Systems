#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <err.h>
#include <stdint.h>
#include <string.h>

int main(const int argc, const char* argv[])
{
	if( argc == 1 ) {
		char* name = NULL;
		size_t len = 0;
		ssize_t length = 0;
		while( (length = getline(&name, &len, stdin)) ) {
			if( length == -1 ) {				
				break;
			}
			
			name[length-1]='\0';
			printf("%s\n", name);	
		}

	} else {
		int rows = 0;
		if ( strcmp(argv[1], "-n") == 0 ) rows = 1;
		
		int CNT = 1;
		int i = 1;
		if(rows) i = 2;
		for(; i<argc; i++) {

			if( strcmp(argv[i],"-") == 0 ) {

				char* name = NULL;
  				size_t len = 0;
				ssize_t length = 0;
  				while( (length = getline(&name, &len, stdin)) ) {
					if( length == -1 ) {				
						break;
					}
			
					name[length-1]='\0';
					if(rows) printf("%d %s\n", CNT, name);
					else printf("%s\n", name);
					CNT++;	
				}

			} else {

				int fd1 = open(argv[i], O_RDONLY);
				if( fd1 == -1 ) {
					warn("Failed to open file %s", argv[i]);
					continue;
				}

				char c;
				char str[4096];
				int j = 0;
				while( read(fd1, &c, sizeof(c)) ) {		
					str[j] = c;
					j++;
		
					if( c=='\n' ) {
						str[j-1]='\0';
						if(rows) printf("%d %s\n", CNT, str);
						else printf("%s\n", str);
						str[0]='\0';
						j=0;
						CNT++;
					}
				}

				close(fd1);
			}
		}
	} 

	exit(0);
}
