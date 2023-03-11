#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>

int main(const int argc, const char* argv[])
{
	if( argc != 3 ) {
		errx(1, "Exactly 2 arguments expected");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) err(1, "Failed to open %s", argv[1]);

	int fd2 = open(argv[2], O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if( fd2 == -1 ) err(1, "Failed to open %s", argv[2]);

	char stdint[1000] = "#include <stdint.h>\n";
	write(fd2, &stdint, strlen(stdint));

	struct stat s;
	stat(argv[1], &s);
	int sz = s.st_size/2;

	if( sz >= 524288 ) {
		errx(1, "524288 uint16_t elements are allowed in the input file. This file has %d", sz);
	}

	uint16_t arr[sz];

	uint16_t c;
	int k = 0;
	while( read(fd1, &c, 2) ) {
		arr[k] = c;
		k++;
	}

	write(fd2, "\nconst uint16_t arr[] = {", 25);
	for(int j=0; j<sz; j++) {
	
		int tmp = arr[j];
		int len = 0;
		while( tmp>0 ) {
			tmp /= 10;
			len++;
		}

		char num[len+1];
		int n = arr[j];

		for(int i=len-1; i>=0; i--) {
			num[i] = (char)((n%10)+48);
			n /= 10;
		}
		num[len] = '\0';

		write(fd2, &num, strlen(num));
		write(fd2, ", ", 2);
	} write(fd2, "};\n", 3);

	int tmpLen = sz;
	int szLen = 0;
	while( tmpLen>0 ) {
		tmpLen /= 10;
		szLen ++;
	}
	char arrN[szLen+1];
	int SZ = sz;
	for(int i=szLen-1; i>=0; i--) {
		arrN[i] = (char)((SZ%10)+48);
		SZ /= 10;
	}
	arrN[szLen] = '\0';

	write(fd2, "\nuint32_t arrN = ", 17);
	write(fd2, &arrN, strlen(arrN));
	write(fd2, ";\n", 2);

	close(fd1);
	close(fd2);
	exit(0);
}
