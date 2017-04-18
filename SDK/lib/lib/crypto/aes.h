/*
 * lib/crypto/aes.h
 */ 


#ifndef AES_H_
#define AES_H_

//#define CBC 1

class Aes
{
public:
	void AES128_ECB_encrypt(unsigned char* input, unsigned char* key, unsigned char* output);
	void AES128_ECB_decrypt(unsigned char* input, unsigned char* key, unsigned char *output);
	void AES128_CBC_encrypt_buffer(unsigned char* output, unsigned char* input, unsigned int length, unsigned char* key, unsigned char* iv);
	void AES128_CBC_decrypt_buffer(unsigned char* output, unsigned char* input, unsigned int length, unsigned char* key, unsigned char* iv);
	
protected:
private:
	unsigned char getSBoxValue(unsigned char num);
	unsigned char getSBoxInvert(unsigned char num);
	void KeyExpansion(void);
	void AddRoundKey(unsigned char round);
	void SubBytes(void);
	void ShiftRows(void);
	unsigned char xtime(unsigned char x);
	void MixColumns(void);
	unsigned char Multiply(unsigned char x, unsigned char y);
	void InvMixColumns(void);
	void InvSubBytes(void);
	void InvShiftRows(void);
	void Cipher(void);
	void InvCipher(void);
	void BlockCopy(unsigned char* output, unsigned char* input);
	void XorWithIv(unsigned char* buf);
	unsigned char* Iv;
};

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "aes.cpp"
#endif
/*#####################################################*/
#endif /* AES_H_ */
