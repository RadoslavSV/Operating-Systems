#include <stdio.h>
#include <stdint.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main(const int argc, const char* argv[])
{
	if( argc != 2 ) {
		errx(1, "The program expects 1 argument, but received %d", argc-1);
	}
	
	int fd1 = open(argv[1], O_RDONLY);
	if( fd1 == -1 ) {
		err(1, "Failed to read file %s", argv[1]);
	}

	char orc[4];
	for(int i=0; i<3; i++) {
		char t;
		if( read(fd1, &t, 1) == -1) {
			err(1, "Failed to read fd1");
		}
		orc[i] = t;
	}
	orc[3] = '\0';
	if( strcmp(orc, "ORC") != 0 ) {
		errx(1, "File %s does not begin with the magic word 'ORC'", argv[1]);
	}

	uint32_t ram_size;
	if( read(fd1, &ram_size, 4) == -1 ) {
		err(1, "Failed to read from fd1");
	}

	int64_t arr[ram_size]; // DECLARE ARRAY WITH SIZE ram_size

	struct stat s;
	if( stat(argv[1], &s) == -1) {
		err(1, "Failed to stat file %s", argv[1]);
	}
	int sz = (s.st_size-7)/25;

	if( (s.st_size-7) % 25 != 0 ) {
		errx(1, "File %s does not contain exact number of operands for each command", argv[1]);
	}

	uint8_t opcode;
	int64_t op1, op2, op3;

	for(int i=0; i<sz; i++) {
		if( read(fd1, &opcode, 1) == -1 ) err(1, "Failed to read opcode from fd1");
		if( read(fd1, &op1, 8) == -1 ) err(1, "Failed to read operand 1 from fd1");
		if( read(fd1, &op2, 8) == -1 ) err(1, "Failed to read operand 2 from fd1");
		if( read(fd1, &op3, 8) == -1 ) err(1, "Failed to read operand 3 from fd1");

		//printf("%d: opcode=%x; op1=%lx, op2=%lx, op3=%lx\n", i, opcode, op1, op2, op3);

		if( opcode == 0x95 ) {
			//set

			if( op1 > ram_size ) errx(1, "Trying to access invalid memory address in 'set'");

			arr[op1] = op2;
		} else if( opcode == 0xc1 ) {
			//out
			
			if( op1 > ram_size ) errx(1, "Trying to access invalid memory address in 'out'");

			char c = arr[op1];
			if( write(1, &c, 1) == -1 ) err(1, "Failed to write in STDOUT in 'out'");
		} else if( opcode == 0x00 ) {
			//nop
	
			// EMPTY	
		} else if( opcode == 0x5d ) {
			//load
			
			if( op1 > ram_size || op2 > ram_size || arr[op2] > ram_size ) errx(1, "Trying to access invalid memory address in 'load'");

			arr[op1] = arr[arr[op2]];
		} else if( opcode == 0x63 ) {
			//store
			
			if( op1 > ram_size || op2 > ram_size || arr[op1] > ram_size ) errx(1, "Trying to access invalid memory address in 'store'");

			arr[arr[op1]] = arr[op2];
		} else if( opcode == 0x91 ) {
			//jmp
		
			if( op1 > ram_size ) errx(1, "Trying to access invalid memory address in 'jmp'");

			int instr = arr[op1];
			
			if( lseek(fd1, 7, SEEK_SET) == -1 ) err(1, "Failed to seek fd1 in 'jmp'");
			if( lseek(fd1, instr*25, SEEK_CUR) == -1 ) err(1, "Failed to seek fd1 in 'jmp'");
			i = instr;
		} else if( opcode == 0x25 ) {
			//sgz

			if( op1 > ram_size ) errx(1, "Trying to access invalid memory address in 'sgz'");
			
			if( arr[op1] > 0 ) {
				if( lseek(fd1, 25, SEEK_CUR) == -1 ) err(1, "Failed to seek fd1 in 'sgz'");
			}
		} else if( opcode == 0xad ) {
			//add
		
			if( op1 > ram_size || op2 > ram_size || op3 > ram_size ) errx(1, "Trying to access invalid memory address in 'add'");

			arr[op1] = arr[op2] + arr[op3];
		} else if( opcode == 0x33 ) {
			//mul

			if( op1 > ram_size || op2 > ram_size || op3 > ram_size ) errx(1, "Trying to access invalid memory address in 'mul'");
		
			arr[op1] = arr[op2] + arr[op3];
		} else if( opcode == 0x04 ) {
			//div
	
			if( op1 > ram_size || op2 > ram_size || op3 > ram_size ) errx(1, "Trying to access invalid memory address in 'div'");
			if( arr[op3] == 0 ) {
				errx(1, "Division by 0");
			}

			arr[op1] = arr[op2] / arr[op3];
		} else if( opcode == 0xb5 ) {
			//mod
			
			if( op1 > ram_size || op2 > ram_size || op3 > ram_size ) errx(1, "Trying to access invalid memory address in 'mod'");
			if( arr[op3] == 0 ) {
				errx(1, "Division by 0");
			}

			arr[op1] = arr[op2] % arr[op3];
		} else if( opcode == 0xbf ) {
			//sleep
		
			if( op1 > ram_size ) errx(1, "Trying to access invalid memory address in 'sleep'");

			int milliseconds = arr[op1]/1000;
			sleep(milliseconds);
		} else {
			errx(1, "Invalid opcode for instruction");
		}
	}

	if( close(fd1) == -1 ) {
		err(1, "Failed to close file %s", argv[1]);
	}
	
	exit(0);
}
