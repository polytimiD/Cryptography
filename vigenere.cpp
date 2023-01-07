#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>

#define columns 5
char ch[32] = "_abcdefghijklmnopqrstuvwxyz,.?!-";
char ciphertext[275];
int key[5] = {12, 15, 22, 5, 30}; // l o v e !

int find_pos(char c){
	int i;

	for(i=0; i< 32; i++){
		if(c == ch[i]){
			return i;
		}
	}

	fprintf(stderr, "\nerror\t%c\t%d\n", c, (unsigned) c);
	exit(0);
	return -1;
}

int main(int argc, char*argv[]){
	int infd, r, i, column, pos;

	if((infd = _open("crypt.txt", _O_RDONLY) ) < 0){
		perror("opening infile");
		return -1;
	}

	if((r = _read(infd, ciphertext, 275)) == -1){
		perror("reading from file");
	}

	for(i=0, column = 0; i< r; i++, column++){
		if(column == columns) column = 0;

		pos = find_pos(ciphertext[i]) - key[column];

		if(pos < 0)
			pos += 32;

		if(pos == 0){
			printf(" "); // print _ as <space>
		}
		else{
			printf("%c", ch[pos]);
		}
	}

	_close(infd);

	return 0;
}
