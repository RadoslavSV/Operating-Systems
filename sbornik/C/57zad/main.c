#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, const char* argv[])
{
	if( argc != 3 ) {
		errx(1, "Input format: ./main <flag/SET1> <SET1/SET2>");
	}

	char str[1000000];
	char c;
	int j = 0;
	while( read(0, &c, 1) ) {
		str[j] = c;
		j++;
	}
	str[j] = '\0';

	//printf("%s", str);

	if( strcmp(argv[1], "-d") == 0 ) {
		
		char newStr[1000000];
		int u = 0;
		for(size_t i=0; i<strlen(str); i++) {
			char m = str[i];
			int Wr = 0;
			for(size_t k=0; k<strlen(argv[2]); k++) {
				char n = argv[2][k];
				if( m == n ) {
					Wr = 1;
					break;
				}
			}
			if( Wr == 0 ) {
				newStr[u] = m;
				u++;
			}
		}
		newStr[u] = '\0';
		printf("%s", newStr);

	} else if( strcmp(argv[1], "-s") == 0 ) {
		
		char newStr[1000000];
		int u = 0;
		size_t i = 0;
		for( ; i<strlen(str); i++) {
			char m = str[i];
			int notIn = 1;
			for(size_t k=0; k<strlen(argv[2]); k++) {
				char n = argv[2][k];
				if( m==n ) {
					newStr[u] = m;
					u++;
					if( str[i+1] == n ) {
						i++;
						while( str[i] == n ) i++;
					} else {
						notIn = 0;
						continue;
					}
					i--;
					notIn = 0;
					break;
				}
			}
			if( notIn ) {
				newStr[u] = m;
				u++;
			}

		}
		newStr[u] = '\0';
		printf("%s", newStr);

	} else {
		
		for(size_t i=0; i<strlen(str); i++) {
			for(size_t k=0; k<strlen(argv[1]); k++) {
				if( str[i] == argv[1][k] ) {
					str[i] = argv[2][k];
				}
			}
		}

		printf("%s", str);
	}

	exit(0);
}
