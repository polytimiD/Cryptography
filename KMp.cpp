#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>


#define XSIZE 1845
char ciphertext[1845];
int starting_at;
char checking[1024];

void preKmp(char *x, int m, int kmpNext[]) {
   int i, j;

   i = 0;
   j = kmpNext[0] = -1;
   while (i < m) {
      while (j > -1 && x[i] != x[j])
         j = kmpNext[j];
      i++;
      j++;
      if (x[i] == x[j])
         kmpNext[i] = kmpNext[j];
      else
         kmpNext[i] = j;
   }
}


void KMP(char *x, int m, char *y, int n) {
   int i, j, kmpNext[XSIZE];

   /* Preprocessing */
   preKmp(x, m, kmpNext);

   /* Searching */
   i = j = 0;
   while (j < n) {
      while (i > -1 && x[i] != y[j])
         i = kmpNext[i];
      i++;
      j++;
      if (i >= m) {
         //OUTPUT(j - i);
		  if(j-i != 0){

			printf("%d\t\t\t%s\n", (j-i), checking);
			//printf("%d\n", (j-i));
		  }

         i = kmpNext[i];
	  }
   }
}


int main(int argc, char* argv[]){
	int infd, r;
	int i, j, k;

	if((infd = _open("crypt.txt", _O_RDONLY) ) < 0){
		perror("opening infile");
		return -1;
	}

	if((r = _read(infd, ciphertext, 1845)) == -1){
		perror("reading from file");
	}

	printf("read %d bytes\n\n", r);


		i = 5;
		printf("for length %d\n", i);
		for(j=0; j< 1845-i; j++){

			starting_at = j;

			for(k=0; k< i; k++)
				checking[k] = ciphertext[j + k];

			checking[k] = '\0';

			KMP(ciphertext+j, i, ciphertext+j, r-j);

		}
		printf("\n----------\n\n");

	return 0;
}