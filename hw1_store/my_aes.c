#include <stdlib.h>
#include <memory.h>
#include "my_aes.h"

#include <stdio.h>

void aes_key_setup(const BYTE key[], WORD w[], int keysize)
{
	int Nb=4,Nr,Nk,idx;
	WORD temp,Rcon[]={0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,
	                  0x40000000,0x80000000,0x1b000000,0x36000000,0x6c000000,0xd8000000,
	                  0xab000000,0x4d000000,0x9a000000};

	switch (keysize) {
		case 128: Nr = 10; Nk = 4; break;
		case 192: Nr = 12; Nk = 6; break;
		case 256: Nr = 14; Nk = 8; break;
		default: return;
	}

	for (idx=0; idx < Nk; ++idx) {
		w[idx] = ((key[4 * idx]) << 24) | ((key[4 * idx + 1]) << 16) |
				   ((key[4 * idx + 2]) << 8) | ((key[4 * idx + 3]));
	}

	for (idx = Nk; idx < Nb * (Nr+1); ++idx) {
		temp = w[idx - 1];
		if ((idx % Nk) == 0)
			temp = SubWord(KE_ROTWORD(temp)) ^ Rcon[(idx-1)/Nk];
		else if (Nk > 6 && (idx % Nk) == 4)
			temp = SubWord(temp);
		w[idx] = w[idx-Nk] ^ temp;
	}
}

void aes_encrypt(const BYTE in[], BYTE out[], const WORD key[], int keysize)
{
	BYTE state[4][4];

	// Copy input array (should be 16 bytes long) to a matrix (sequential bytes are ordered
	// by row, not col) called "state" for processing.
	// *** Implementation note: The official AES documentation references the state by
	// column, then row. Accessing an element in C requires row then column. Thus, all state
	// references in AES must have the column and row indexes reversed for C implementation.
	state[0][0] = in[0];
	state[1][0] = in[1];
	state[2][0] = in[2];
	state[3][0] = in[3];
	state[0][1] = in[4];
	state[1][1] = in[5];
	state[2][1] = in[6];
	state[3][1] = in[7];
	state[0][2] = in[8];
	state[1][2] = in[9];
	state[2][2] = in[10];
	state[3][2] = in[11];
	state[0][3] = in[12];
	state[1][3] = in[13];
	state[2][3] = in[14];
	state[3][3] = in[15];

	// Perform the necessary number of rounds. The round key is added first.
	// The last round does not perform the MixColumns step.
	AddRoundKey(state,&key[0]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[4]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[8]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[12]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[16]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[20]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[24]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[28]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[32]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[36]);
	if (keysize != 128) {
		SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[40]);
		SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[44]);
		if (keysize != 192) {
			SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[48]);
			SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state,&key[52]);
			SubBytes(state); ShiftRows(state); AddRoundKey(state,&key[56]);
		}
		else {
			SubBytes(state); ShiftRows(state); AddRoundKey(state,&key[48]);
		}
	}
	else {
		SubBytes(state); ShiftRows(state); AddRoundKey(state,&key[40]);
	}

	// Copy the state to the output array.
	out[0] = state[0][0];
	out[1] = state[1][0];
	out[2] = state[2][0];
	out[3] = state[3][0];
	out[4] = state[0][1];
	out[5] = state[1][1];
	out[6] = state[2][1];
	out[7] = state[3][1];
	out[8] = state[0][2];
	out[9] = state[1][2];
	out[10] = state[2][2];
	out[11] = state[3][2];
	out[12] = state[0][3];
	out[13] = state[1][3];
	out[14] = state[2][3];
	out[15] = state[3][3];
}

void aes_decrypt(const BYTE in[], BYTE out[], const WORD key[], int keysize)
{
	BYTE state[4][4];

	// Copy the input to the state.
	state[0][0] = in[0];
	state[1][0] = in[1];
	state[2][0] = in[2];
	state[3][0] = in[3];
	state[0][1] = in[4];
	state[1][1] = in[5];
	state[2][1] = in[6];
	state[3][1] = in[7];
	state[0][2] = in[8];
	state[1][2] = in[9];
	state[2][2] = in[10];
	state[3][2] = in[11];
	state[0][3] = in[12];
	state[1][3] = in[13];
	state[2][3] = in[14];
	state[3][3] = in[15];

	// Perform the necessary number of rounds. The round key is added first.
	// The last round does not perform the MixColumns step.
	if (keysize > 128) {
		if (keysize > 192) {
			AddRoundKey(state,&key[56]);
			InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[52]);InvMixColumns(state);
			InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[48]);InvMixColumns(state);
		}
		else {
			AddRoundKey(state,&key[48]);
		}
		InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[44]);InvMixColumns(state);
		InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[40]);InvMixColumns(state);
	}
	else {
		AddRoundKey(state,&key[40]);
	}
	InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[36]);InvMixColumns(state);
	InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[32]);InvMixColumns(state);
	InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[28]);InvMixColumns(state);
	InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[24]);InvMixColumns(state);
	InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[20]);InvMixColumns(state);
	InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[16]);InvMixColumns(state);
	InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[12]);InvMixColumns(state);
	InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[8]);InvMixColumns(state);
	InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[4]);InvMixColumns(state);
	InvShiftRows(state);InvSubBytes(state);AddRoundKey(state,&key[0]);

	// Copy the state to the output array.
	out[0] = state[0][0];
	out[1] = state[1][0];
	out[2] = state[2][0];
	out[3] = state[3][0];
	out[4] = state[0][1];
	out[5] = state[1][1];
	out[6] = state[2][1];
	out[7] = state[3][1];
	out[8] = state[0][2];
	out[9] = state[1][2];
	out[10] = state[2][2];
	out[11] = state[3][2];
	out[12] = state[0][3];
	out[13] = state[1][3];
	out[14] = state[2][3];
	out[15] = state[3][3];
}

void xor(const BYTE in[], BYTE out[], size_t len) {
    for(size_t i=0; i<len; i++) {
        out[i] ^= in[i];
    }

}


// the following is implemented by myself
int aes_encrypt_cbc(FILE *in, FILE *out, const WORD key[], int keysize, const BYTE iv[]) {
    BYTE buf_in[AES_BLOCK_SIZE], buf_out[AES_BLOCK_SIZE], buf_iv[AES_BLOCK_SIZE];

    memcpy(buf_iv, iv, AES_BLOCK_SIZE);
    size_t read_size;
    int pad = 0;

    while(read_size = fread(buf_in, 1, AES_BLOCK_SIZE, in)) {
        if(read_size < AES_BLOCK_SIZE) {
            int fill_byte = AES_BLOCK_SIZE - read_size;
            memset(buf_in+read_size, fill_byte, AES_BLOCK_SIZE);
            pad = 1;
        }
        xor(buf_iv, buf_in, AES_BLOCK_SIZE);
        aes_encrypt(buf_in, buf_out, key, keysize);
        memcpy(buf_iv, buf_out, AES_BLOCK_SIZE);
        fwrite(buf_out, 1, AES_BLOCK_SIZE, out);
    }
    if (!pad) {
        memset(buf_in, AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        xor(buf_iv, buf_in, AES_BLOCK_SIZE);
        aes_encrypt(buf_in, buf_out, key, keysize);
        fwrite(buf_out, 1, AES_BLOCK_SIZE, out);
    }

    return 1;
}

int aes_decrypt_cbc(FILE *in, FILE *out, const WORD key[], int keysize, const BYTE iv[]) {
    BYTE buf_in[AES_BLOCK_SIZE], buf_out[AES_BLOCK_SIZE], buf_iv[AES_BLOCK_SIZE];

    memcpy(buf_iv, iv, AES_BLOCK_SIZE);

    while(fread(buf_in, 1, AES_BLOCK_SIZE, in)) {
        aes_decrypt(buf_in, buf_out, key, keysize);
        xor(buf_iv, buf_out, AES_BLOCK_SIZE);
        memcpy(buf_iv, buf_in, AES_BLOCK_SIZE);
        fwrite(buf_out, 1, AES_BLOCK_SIZE, out);
    }

    return 1;

}