#include <stdio.h>
#include <stdlib.h>

char word[8] = { 'o','t','t','o','m','a','n','o'};
char key[10] = {'i','t','n','a','u','g','o','t','t','o'};


unsigned char bit_to_byte(char *arr, int index) {
	int i;
	unsigned char byte = 0x00;
	for (i = 0; i < 8; i++) {
		byte = byte | (*(arr+index+i) << i);
	}

	return byte;
}


int main(void) {
	char ptext[64], tmptext[64];
	char Mkey[80], tmpkey[80];
	char b[4];
	char tmp, tmp2;

	int i, j, k;

/*-------------------------------INPUT-BITSLICING-------------------------------*/

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			ptext[i * 8 + j] = (word[i] & (0x01 << j)) >> j;
		}
	}

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 8; j++) {
			Mkey[i * 8 + j] = (key[i] & (0x01 << j)) >> j;
		}
	}
/*
	printf("ROUNDKEY: ");
	for (i = 0; i < 64; i += 8) {
		printf("%x ", bit_to_byte(&Mkey[0]+16, i));
	}
	printf("\n");
*/

	printf("PLAINTEXT: ");
	for (i = 0; i < 64; i += 8) {
		printf("%x ", bit_to_byte(&ptext[0], i));
	}
	printf("\n");


	for (k = 0; k < 32; k++) {
		
		/*--------------------------------------ADDROUNDKEY------------------------------------------*/

		for (j = 0; j < 64; j++) {
			ptext[j] = ptext[j] ^ Mkey[j + 16];
		}


/*
		printf("ADDROUNDKEY: ");
		for (i = 0; i < 64; i += 8) {
			printf("%x ", bit_to_byte(&ptext[0], i));
		}
		printf("\n");
*/
		/*--------------------------------------SBOX-------------------------------------------------*/

		for (i = 0; i < 64; i += 4) {
			b[0] = ptext[i + 3];
			b[1] = ptext[i + 2];
			b[2] = ptext[i + 1];
			b[3] = ptext[i];
			ptext[i + 3] = ((b[0] ^ 0x01) & (b[2] ^ 0x01) & (b[3] ^ 0x01)) |
				((b[0] ^ 0x01) & b[2] & b[3]) |
				((b[0] ^ 0x01)&b[1] & b[2] & (b[3] ^ 0x01)) |
				(b[0] & (b[1] ^ 0x01)&b[3]) |
				(b[0] & (b[1] ^ 0x01)&b[2] & (b[3] ^ 0x01));

			ptext[i + 2] = ((b[0] ^ 0x01)&(b[1] ^ 0x01)&(b[2] ^ 0x01)) |
				((b[1] ^ 0x01)&b[2] & (b[3] ^ 0x01)) |
				((b[0] ^ 0x01)&b[1] & b[2] & b[3]) |
				(b[0] & (b[2] ^ 0x01)&b[3]) |
				(b[0] & b[1] & (b[2] ^ 0x01)&(b[3] ^ 0x01));

			ptext[i + 1] = ((b[0] ^ 0x01)&(b[1] ^ 0x01)&b[2]) |
				((b[0] ^ 0x01)&b[1] & b[2] & (b[3] ^ 0x01)) |
				(b[0] & (b[1] ^ 0x01)&(b[2] ^ 0x01)) |
				(b[0] & (b[1] ^ 0x01)&b[2] & (b[3] ^ 0x01)) |
				(b[0] & b[1] & b[3]);

			ptext[i] = ((b[0] ^ 0x01)&(b[1] ^ 0x01)&b[3]) |
				((b[0] ^ 0x01)&b[1] & (b[2] ^ 0x01)&(b[3] ^ 0x01)) |
				((b[0] ^ 0x01)&b[1] & b[2] & b[3]) |
				(b[0] & (b[1] ^ 0x01)&(b[3] ^ 0x01)) |
				(b[0] & b[1] & (b[2] ^ 0x01)&b[3]) |
				(b[0] & b[1] & b[2] & (b[3] ^ 0x01));
		}
/*
		printf("SBOX: ");
		for (i = 0; i < 64; i += 8) {
			printf("%x ", bit_to_byte(&ptext[0], i));
		}
		printf("\n");
*/
		/*---------------------------------------PBOX--------------------------------------------*/

		for (i = 0; i < 64; i++) {
			tmptext[i] = ptext[i];
		}

		for (i = 0; i < 64; i++) {
			ptext[i] = tmptext[(i % 16) * 4 + i / 16];
		}

/*
		printf("PBOX: ");
		for (i = 0; i < 64; i += 8) {
			printf("%x ", bit_to_byte(&ptext[0], i));
		}
		printf("\n");
*/
		/*---------------------------------------KEY-UPDATE--------------------------------------*/

		for (i = 0; i < 80; i++) {
			tmpkey[i] = Mkey[i];
		}
/*
		printf("MASTERKEY round%d: ", k);
		for (i = 0; i < 80; i += 8) {
			printf("%x ", bit_to_byte(&Mkey[0], i));
		}
		printf("\n");
*/
		for (i = 0; i < 80; i++) {
			Mkey[i] = tmpkey[(i + 19) % 80];
		}
/*
		printf("ROTATED MASTERKEY: ");
		for (i = 0; i < 80; i += 8) {
			printf("%x ", bit_to_byte(&Mkey[0], i));
		}
		printf("\n");
*/


		b[0] = Mkey[79];
		b[1] = Mkey[78];
		b[2] = Mkey[77];
		b[3] = Mkey[76];
		Mkey[79] = ((b[0] ^ 0x01) & (b[2] ^ 0x01) & (b[3] ^ 0x01)) |
			((b[0] ^ 0x01) & b[2] & b[3]) |
			((b[0] ^ 0x01)&b[1] & b[2] & (b[3] ^ 0x01)) |
			(b[0] & (b[1] ^ 0x01)&b[3]) |
			(b[0] & (b[1] ^ 0x01)&b[2] & (b[3] ^ 0x01));

		Mkey[78] = ((b[0] ^ 0x01)&(b[1] ^ 0x01)&(b[2] ^ 0x01)) |
			((b[1] ^ 0x01)&b[2] & (b[3] ^ 0x01)) |
			((b[0] ^ 0x01)&b[1] & b[2] & b[3]) |
			(b[0] & (b[2] ^ 0x01)&b[3]) |
			(b[0] & b[1] & (b[2] ^ 0x01)&(b[3] ^ 0x01));

		Mkey[77] = ((b[0] ^ 0x01)&(b[1] ^ 0x01)&b[2]) |
			((b[0] ^ 0x01)&b[1] & b[2] & (b[3] ^ 0x01)) |
			(b[0] & (b[1] ^ 0x01)&(b[2] ^ 0x01)) |
			(b[0] & (b[1] ^ 0x01)&b[2] & (b[3] ^ 0x01)) |
			(b[0] & b[1] & b[3]);

		Mkey[76] = ((b[0] ^ 0x01)&(b[1] ^ 0x01)&b[3]) |
			((b[0] ^ 0x01)&b[1] & (b[2] ^ 0x01)&(b[3] ^ 0x01)) |
			((b[0] ^ 0x01)&b[1] & b[2] & b[3]) |
			(b[0] & (b[1] ^ 0x01)&(b[3] ^ 0x01)) |
			(b[0] & b[1] & (b[2] ^ 0x01)&b[3]) |
			(b[0] & b[1] & b[2] & (b[3] ^ 0x01));
/*
		printf("TOPSBOXED MKEY: ");
		for (i = 0; i < 80; i += 8) {
			printf("%x ", bit_to_byte(&Mkey[0], i));
		}
		printf("\n");
*/
		for (i = 0; i < 5; i++) {
			Mkey[i + 15] = Mkey[i + 15] ^ ((k&(0x01 << i)) >> i);
		}
/*
		printf("COUNTERXORED MKEY: ");
		for (i = 0; i < 80; i += 8) {
			printf("%x ", bit_to_byte(&Mkey[0], i));
		}
		printf("\n");
*/
	}
/*
	printf("PLAINTEXT: ");
	for (i = 0; i < 64; i += 8) {
		printf("%x ", bit_to_byte(&ptext[0], i));
	}
	printf("\n");
*/

	/*--------------------------------------ADDROUNDKEY------------------------------------------*/

	for (j = 0; j < 64; j++) {
		ptext[j] = ptext[j] ^ Mkey[j + 16];
	}

	printf("CIPHERTEXT: ");
	for (i = 0; i < 64; i += 8) {
		printf("%x ", bit_to_byte(&ptext[0], i));
	}
	printf("\n");

	/*--------------------------------------ADDROUNDKEY------------------------------------------*/

	for (j = 0; j < 64; j++) {
		ptext[j] = ptext[j] ^ Mkey[j + 16];
	}
/*
	printf("PLAINTEXT: ");
	for (i = 0; i < 64; i += 8) {
		printf("%x ", bit_to_byte(&ptext[0], i));
	}
	printf("\n");
*/
	for (k = 31; k >= 0; k--) {

	/*--------------------------------------KEY-UPDATE-------------------------------------------*/

		for (i = 0; i < 5; i++) {
			Mkey[i + 15] = Mkey[i + 15] ^ ((k&(0x01 << i)) >> i);
		}

		b[3] = Mkey[79];
		b[2] = Mkey[78];
		b[1] = Mkey[77];
		b[0] = Mkey[76];

		Mkey[79] = ((b[3] ^ 0x01)&(b[2] ^ 0x01)&b[0]) |
			((b[3] ^ 0x01)&(b[2] ^ 0x01)&b[1] & (b[0] ^ 0x01)) |
			((b[3] ^ 0x01)&b[2] & (b[1] ^ 0x01)&(b[0] ^ 0x01)) |
			((b[3] ^ 0x01)&b[2] & b[1] & b[0]) |
			(b[3] & (b[2] ^ 0x01)&(b[1] ^ 0x01)&(b[0] ^ 0x01)) |
			(b[3] & b[2] & b[1]);

		Mkey[78] = ((b[3] ^ 0x01)&(b[2] ^ 0x01)&(b[1] ^ 0x01)) |
			((b[2] ^ 0x01)&b[1] & (b[0] ^ 0x01)) |
			((b[3] ^ 0x01)&b[2] & (b[1] ^ 0x01)&(b[0] ^ 0x01)) |
			((b[3] ^ 0x01)&b[2] & b[1] & b[0]) |
			(b[3] & (b[1] ^ 0x01)&b[0]);

		Mkey[77] = ((b[3] ^ 0x01)&(b[2] ^ 0x01)&(b[1] ^ 0x01)&b[0]) |
			((b[3] ^ 0x01)&b[1] & (b[0] ^ 0x01)) |
			(b[3] & (b[2] ^ 0x01)&(b[0] ^ 0x01)) |
			(b[3] & b[1] & b[0]) |
			(b[3] & b[2] & (b[1] ^ 0x01)&b[0]);

		Mkey[76] = ((b[3] ^ 0x01)&(b[2] ^ 0x01)&(b[0] ^ 0x01)) |
			((b[3] ^ 0x01)&b[2] & b[0]) |
			(b[3] & (b[2] ^ 0x01)&(b[1] ^ 0x01)&(b[0] ^ 0x01)) |
			(b[3] & (b[2] ^ 0x01)&b[1] & b[0]) |
			(b[3] & b[2] & (b[1] ^ 0x01)&b[0]) |
			(b[3] & b[2] & b[1] & (b[0] ^ 0x01));


		for (i = 0; i < 80; i++) {
			tmpkey[i] = Mkey[i];
		}

		for (i = 0; i < 80; i++) {
			Mkey[i] = tmpkey[(i + 61) % 80];
		}
		

/*
		printf("MKEY: ");
		for (i = 0; i < 80; i += 8) {
			printf("%x ", bit_to_byte(&tmpkey[0], i));
		}
		printf("\n");

		printf("ROTATED MKEY: ");
		for (i = 0; i < 80; i += 8) {
			printf("%x ", bit_to_byte(&Mkey[0], i));
		}
		printf("\n");
*/
/*-------------------------------------PBOX----------------------------------------*/

		for (i = 0; i < 64; i++) {
			tmptext[i] = ptext[i];
		}

		for (i = 0; i < 64; i++) {
			//if (j >= 64) j = (j % 64) + 1;
			ptext[i] = tmptext[(i % 4) * 16 + i / 4];
		}
/*
		printf("SBOX: ");
		for (i = 0; i < 64; i += 8) {
			printf("%x ", bit_to_byte(&ptext[0], i));
		}
		printf("\n");
*/		

/*-------------------------------------SBOX----------------------------------------*/
		for (i = 0; i < 64; i+=4) {
			b[3] = ptext[i + 3];
			b[2] = ptext[i + 2];
			b[1] = ptext[i + 1];
			b[0] = ptext[i];

			ptext[i + 3] = ((b[3] ^ 0x01)&(b[2] ^ 0x01)&b[0]) |
				((b[3] ^ 0x01)&(b[2] ^ 0x01)&b[1] & (b[0] ^ 0x01)) |
				((b[3] ^ 0x01)&b[2] & (b[1] ^ 0x01)&(b[0] ^ 0x01)) |
				((b[3] ^ 0x01)&b[2] & b[1] & b[0]) |
				(b[3] & (b[2] ^ 0x01)&(b[1] ^ 0x01)&(b[0] ^ 0x01)) |
				(b[3] & b[2] & b[1]);

			ptext[i + 2] = ((b[3] ^ 0x01)&(b[2] ^ 0x01)&(b[1] ^ 0x01)) |
				((b[2] ^ 0x01)&b[1] & (b[0] ^ 0x01)) |
				((b[3] ^ 0x01)&b[2] & (b[1] ^ 0x01)&(b[0] ^ 0x01)) |
				((b[3] ^ 0x01)&b[2] & b[1] & b[0]) |
				(b[3] & (b[1] ^ 0x01)&b[0]);

			ptext[i + 1] = ((b[3] ^ 0x01)&(b[2] ^ 0x01)&(b[1] ^ 0x01)&b[0]) |
				((b[3] ^ 0x01)&b[1] & (b[0] ^ 0x01)) |
				(b[3] & (b[2] ^ 0x01)&(b[0] ^ 0x01)) |
				(b[3] & b[1] & b[0]) |
				(b[3] & b[2] & (b[1] ^ 0x01)&b[0]);

			ptext[i] = ((b[3] ^ 0x01)&(b[2] ^ 0x01)&(b[0] ^ 0x01)) |
				((b[3] ^ 0x01)&b[2] & b[0]) |
				(b[3] & (b[2] ^ 0x01)&(b[1] ^ 0x01)&(b[0] ^ 0x01)) |
				(b[3] & (b[2] ^ 0x01)&b[1] & b[0]) |
				(b[3] & b[2] & (b[1] ^ 0x01)&b[0]) |
				(b[3] & b[2] & b[1] & (b[0] ^ 0x01));
		}
/*
		printf("UNSBOXED TEXT: ");
		for (i = 0; i < 64; i += 8) {
			printf("%x ", bit_to_byte(&ptext[0], i));
		}
		printf("\n");
*/
/*--------------------------------ADDROUNDKEY------------------------------------*/

		for (i = 0; i < 64; i++) {
			ptext[i] = ptext[i] ^ Mkey[i + 16];
		}
/*
		printf("ROUNDKEY: ");
		for (i = 0; i < 64; i += 8) {
			printf("%x ", bit_to_byte(&Mkey[0] + 16, i));
		}
		printf("\n");

		printf("UNXORED TEXT: ");
		for (i = 0; i < 64; i += 8) {
			printf("%x ", bit_to_byte(&ptext[0], i));
		}
		printf("\n");
*/
	}

	printf("PLAINTEXT: ");
	for (i = 0; i < 64; i += 8) {
		printf("%x ", bit_to_byte(&ptext[0], i));
	}

	return 0;
}