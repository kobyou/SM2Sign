#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "Sm2Ctrl.h"
#include "Sm2.h"
#include "Sm3.h"
#include "Sm4.h"


/*
���ܣ�����ָ�����������
���룺len���������
�����rand�����
���أ���
*/
void SM2_gen_rand(unsigned char rnd[], int len)
{
#if 0
	pid_t pid = getpid();
	int randData;
	int n = 0;

	srand((unsigned int)pid);
	while(len >= 4) {
		randData = rand();
		rnd[n++] = (unsigned char)(randData >> 24);
		rnd[n++] = (unsigned char)(randData >> 16);
		rnd[n++] = (unsigned char)(randData >> 8);
		rnd[n++] = (unsigned char)(randData);

		len -= 4;
	}

	if(len > 0) {
		randData = rand();
		for(len = len; len > 0; len--) rnd[n++] = (unsigned char)(randData >> (8 * len));
	}
#else
	int randData;
	int n = 0;
	srand((unsigned int)time(NULL));
	for(len = len; len > 0; len--) {
		randData = rand() % 256;
		rnd[n++] = (unsigned char)(randData);
	}
#endif
}

/*
���ܣ�SM2�㷨��Բ���߲�����ʼ��
���룺��
�������
���أ���
*/
void SM2_init(void)
{
	static unsigned char fRunOnce = 0;
	int iret;

	if(0 == fRunOnce) {
		if(0 != (iret = SM2_standard_init())) printf("[ERROR]%s %s: SM2_init() error\n", __FILE__, __LINE__);

		fRunOnce = 1;
	}
}


/*
���ܣ�����SM2�㷨��˽��Կ��[d��G(x,y)]
���룺rand��ѡ�����
�����privkey˽Կd��px��ԿGx��py��ԿGy
���أ�0�ɹ� !0ʧ��
*/
int SM2_keypair_generation(unsigned char rand[], unsigned char privkey[], unsigned char px[], unsigned char py[])
{
	unsigned char seed[32];

	SM2_init();

	if(NULL == rand) SM2_gen_rand(seed, 32);
	else memcpy(seed, rand, 32);

	//˽Կ������[1,n-2]
	while(0 != Test_PrivKey(seed)) SM2_gen_rand(seed, 32);

	memcpy(privkey, seed, 32);

	SM2_keygeneration_2(privkey, px, py);

	return 0;
}

/*
���ܣ�Ԥ�����ɹ�Կ����ZA
���룺px��ԿGx��py��ԿGy
�����za
���أ���
*/
void SM2_sign_pre(unsigned char px[], unsigned char py[], unsigned char za[])
{
	SM2_pre_ZA(px, py, za);
}


/*
���ܣ�SM2�㷨˽Կǩ��
���룺privkey˽Կd��zaԤ����ֵ��msg��Ϣ��msglen��Ϣ���ȡ�rand��ѡ�����
�����signRǩ��R��signSǩ��S
���أ�0�ɹ� !0ʧ��
*/
int SM2_sign(unsigned char privkey[], unsigned char za[], unsigned char msg[], int msglen, unsigned char rand[], unsigned char signR[], unsigned char signS[])
{
	unsigned char seed[32];

	SM2_init();

	if(NULL == rand) SM2_gen_rand(seed, sizeof(seed));
	else memcpy(seed, rand, sizeof(seed));

	return SM2_standard_sign(msg, msglen, za, seed, privkey, signR, signS);
}

/*
���ܣ�SM2�㷨��Կ��֤ǩ��
���룺px��ԿGx��py��ԿGy��msg��Ϣ��msglen��Ϣ���ȡ�signRǩ��R��signSǩ��S
�������
���أ�0�ɹ� !0ʧ��
*/
int SM2_verify(unsigned char px[], unsigned char py[], unsigned char za[], unsigned char msg[], int msglen, unsigned char signR[], unsigned char signS[])
{
	SM2_init();

	return SM2_standard_verify(msg, msglen, za, px, py, signR, signS);
}

/*
���ܣ�SM2�㷨��Կ����
���룺px��ԿGx��py��ԿGy��plain���ġ�plainlen���ĳ��ȡ�rand��ѡ�����
�����cipher����
���أ�0�ɹ� !0ʧ��
*/
int SM2_encrypt(unsigned char px[], unsigned char py[], unsigned char plain[], int plainlen, unsigned char rand[], unsigned char cipher[])
{
	unsigned char seed[32];

	SM2_init();

	if(NULL == rand) SM2_gen_rand(seed, sizeof(seed));
	else memcpy(seed, rand, sizeof(seed));

	return SM2_standard_encrypt_2(seed, px, py, plain, plainlen, cipher);
}

/*
���ܣ�SM2�㷨˽Կ����
���룺privkey˽Կd��cipher���ġ�cipherlen���ĳ���
�����plain����
���أ�0�ɹ� !0ʧ��
*/
int SM2_decrypt(unsigned char privkey[], unsigned char cipher[], int cipherlen, unsigned char plain[])
{
	SM2_init();

	return SM2_standard_decrypt_2(privkey, cipher, cipherlen, plain);
}


/*
���ܣ�SM3�㷨�����ݽ���HASH����
���룺msg��Ϣ��mlen��Ϣ����
�����hashժҪֵ
���أ���
*/
void SM3(unsigned char msg[], int mlen, unsigned char hash[])
{
	SM3_256(msg, mlen, hash);
}


/*
���ܣ�SM4�㷨����
���룺in���ġ�inlen���ĳ��ȡ�key��Կ
�����out����
���أ�0�ɹ� !0ʧ��
*/
int SM4_encrypt(const unsigned char in[], int inlen, unsigned char out[], const unsigned char key[])
{
	int i = 0;
	if(0 != (inlen % 16)) return -1;

	while(inlen > 0) {
		SMS4_encrypt(in[i], out[i], key);

		i += 16;
		inlen -= 16;
	}

	return 0;
}


/*
���ܣ�SM4�㷨����
���룺in���ġ�inlen���ĳ��ȡ�key��Կ
�����out����
���أ�0�ɹ� !0ʧ��
*/
int SM4_decrypt(const unsigned char in[], int inlen, unsigned char out[], const unsigned char key[])
{

	int i = 0;
	if(0 != (inlen % 16)) return -1;

	while(inlen > 0) {
		SMS4_decrypt(in[i], out[i], key);

		i += 16;
		inlen -= 16;
	}

	return 0;
}
