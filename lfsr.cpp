#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

typedef unsigned __int8 uint8;

uint8 reg_length[5] = {-1, 4, 5, 7, 9}; //reg_length[0] unused

uint8 reg1[4];
uint8 reg2[5];
uint8 reg3[7];
uint8 reg4[9];

uint8 original_keystream[30177105];
uint8 new_stream[30177105];
int ones;
int xor_ones;


uint8 *select_register(int regid){
	uint8* reg;

	switch(regid){
		case 1:
			reg = reg1;
			break;
		case 2:
			reg = reg2;
			break;
		case 3:
			reg = reg3;
			break;
		case 4:
			reg = reg4;
			break;
		default:
			fprintf(stderr, "bad register number\n");
			exit(EXIT_FAILURE);
	}

	return reg;
}

void init_register(int regid, uint8 *data){
	int len;
	uint8 *reg;
	int i;

	len = reg_length[regid];
	reg = select_register(regid);

	for(i=0; i< len; i++){
		reg[i] = data[i];
	}
}

uint8 lfsr(int regid){
	int len;
	uint8 *reg;
	uint8 result;
	uint8 f;
	int i;

	reg = select_register(regid);
	len = reg_length[regid];

	result = reg[len-1];

	//count f
	switch(regid){
		case 1:
			f = reg[2] ^ reg[3];
			break;
		case 2:
			f = reg[1] ^ reg[4];
			break;
		case 3:
			f = reg[3] ^ reg[6];
			break;
		case 4:
			f = reg[4] ^ reg[8];
			break;
		default:
			fprintf(stderr, "bad register number\n");
			exit(EXIT_FAILURE);
	}

	//shift
	for(i=len-1; i > 0; i--){
		reg[i] = reg[i-1];
	}

	reg[0] = f;

	return result;
}

int load_original_keystream(){
	int f;

	if((f = _open("keystream.dat", _O_RDONLY | _O_BINARY)) == -1){
		perror("Couldn't open keystream.dat");
		return -1;
	}

	if(_read(f, original_keystream, 30177105) != 30177105){
		perror("Coyldn't load whole the file");
		_close(f);
		return -1;
	}

	close(f);

	return 0;
}

void get_next_init_vector(int regid, uint8 *init_vec){
	int len = reg_length[regid];
	uint8 *reg;
	int i;

	reg = select_register(regid);

	for(i=len-1; i >=0; i--){
		if(init_vec[i] == 1){
			init_vec[i] = 0;
		}
		else{
			//init_vec[i] == 0
			init_vec[i] = 1;
			break;
		}
	}
}

void display_init_vector(int regid, uint8 *init_vec){
	int len = reg_length[regid];
	int i;

	for(i=0; i< len; i++){
		printf("%d", init_vec[i]);
	}
}

void generate_new_stream(int regid){
	register int i;
	ones = 0;

	for(i=0; i< 30177105; i++){
		new_stream[i] = lfsr(regid);
		/*if((new_stream[i] = lfsr(regid)) == 1){
			ones++;
		}*/
	}
}

void xor_and_count_zeros(){
	register int i;

	xor_ones = 0;
	for(i=0; i< 30177105; i++){
		if((original_keystream[i] ^ new_stream[i]) == 1){
			xor_ones++;
		}
	}

}


int main(int argc, char* argv[]){
	int i;
	uint8 init_vector1[] = {0, 0, 0, 1};
	uint8 init_vector2[] = {0, 0, 0, 0, 1};
	uint8 init_vector3[] = {0, 0, 0, 0, 0, 0, 1};
	uint8 init_vector4[] = {0, 0, 0, 0, 0, 0, 0, 0, 1};


	printf("loading file : %s\n", load_original_keystream() == 0 ? "OK" : "GTP");

	//first register
	for(i=0; i < 15; i++){
		init_register(1, init_vector1);
		display_init_vector(1, init_vector1);
		generate_new_stream(1);
		xor_and_count_zeros();
		printf("\t%d ones", xor_ones);
		get_next_init_vector(1, init_vector1);
		printf("\n");
	}


	//second register
	for(i=0; i < 31; i++){
		init_register(2, init_vector2);
		display_init_vector(2, init_vector2);
		generate_new_stream(2);
		xor_and_count_zeros();
		printf("\t%d ones", xor_ones);
		get_next_init_vector(2, init_vector2);
		printf("\n");
	}
		//first register
	for(i=0; i < 127; i++){
		init_register(3, init_vector3);
		display_init_vector(3, init_vector3);
		generate_new_stream(3);
		xor_and_count_zeros();
		printf("\t%d ones", xor_ones);
		get_next_init_vector(3, init_vector3);
		printf("\n");
	}
		//first register
	for(i=0; i < 511; i++){
		init_register(4, init_vector4);
		display_init_vector(4, init_vector4);
		generate_new_stream(4);
		xor_and_count_zeros();
		printf("\t%d ones", xor_ones);
		get_next_init_vector(4, init_vector4);
		printf("\n");
	}
	return 0;
}

