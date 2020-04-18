#ifndef SM2_CTRL_H
#define SM2_CTRL_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


//hex�ַ���ת�ֽ���
static void hex2bytes(const char* hex, int hlen, unsigned char* bytes)
{
	int n;
	char cH4, cL4;

	for(n = 0; n < hlen / 2; n++) {
		cH4 = hex[2 * n];
		cL4 = hex[2 * n + 1];

		cH4 = (cH4 >= '0' && cH4 <= '9') ? (cH4 - '0') : (cH4);
		cH4 = (cH4 >= 'a' && cH4 <= 'f') ? (cH4 - 'a' + 10) : (cH4);
		cH4 = (cH4 >= 'A' && cH4 <= 'F') ? (cH4 - 'A' + 10) : (cH4);

		cL4 = (cL4 >= '0' && cL4 <= '9') ? (cL4 - '0') : (cL4);
		cL4 = (cL4 >= 'a' && cL4 <= 'f') ? (cL4 - 'a' + 10) : (cL4);
		cL4 = (cL4 >= 'A' && cL4 <= 'F') ? (cL4 - 'A' + 10) : (cL4);

		bytes[n] = (unsigned char)(cH4 << 4 | cL4);
	}
}

//�ֽ���תhex�ַ���
static void bytes2hex(const unsigned char* bytes, int blen, char* hex)
{
	int n, m = 0;
	char hexMap[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	for(n = 0; n < blen; n++) {
		hex[m++] = hexMap[(bytes[n] >> 4) & 0x0f];
		hex[m++] = hexMap[bytes[n] & 0x0f];
	}
}

static void dump_byte(const unsigned char* bytes, int blen)
{
	const unsigned char* p = bytes;

	while(blen > 16) {
		printf("\t%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
		       p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
		blen -= 16;
		p += 16;
	}

	if(blen) {
		printf("\t");
		while(blen--)	printf("%02x ", *p++);
		printf("\n");
	}

	printf("\n");
}


/*
���ܣ�����ָ�����������
���룺len���������
�����rand�����
���أ���
*/
void SM2_gen_rand(unsigned char rnd[], int len);

/*
���ܣ�SM2�㷨��Բ���߲�����ʼ��
���룺��
�������
���أ���
*/
void SM2_init(void);

/*
���ܣ�����SM2�㷨��˽��Կ��[d��G(x,y)]
���룺rand��ѡ�����
�����privkey˽Կd��px��ԿGx��py��ԿGy
���أ�0�ɹ� !0ʧ��
*/
int SM2_keypair_generation(unsigned char rand[], unsigned char privkey[], unsigned char px[], unsigned char py[]);

/*
���ܣ�Ԥ�����ɹ�Կ����ZA
���룺px��ԿGx��py��ԿGy
�����za
���أ���
*/
void SM2_sign_pre(unsigned char px[], unsigned char py[], unsigned char za[]);

/*
���ܣ�SM2�㷨˽Կǩ��
���룺privkey˽Կd��zaԤ����ֵ��msg��Ϣ��msglen��Ϣ���ȡ�rand��ѡ�����
�����signRǩ��R��signSǩ��S
���أ�0�ɹ� !0ʧ��
*/
int SM2_sign(unsigned char privkey[], unsigned char za[], unsigned char msg[], int msglen, unsigned char rand[], unsigned char signR[], unsigned char signS[]);

/*
���ܣ�SM2�㷨��Կ��֤ǩ��
���룺px��ԿGx��py��ԿGy��msg��Ϣ��msglen��Ϣ���ȡ�signRǩ��R��signSǩ��S
�������
���أ�0�ɹ� !0ʧ��
*/
int SM2_verify(unsigned char px[], unsigned char py[], unsigned char za[], unsigned char msg[], int msglen, unsigned char signR[], unsigned char signS[]);

/*
���ܣ�SM2�㷨��Կ����
���룺px��ԿGx��py��ԿGy��plain���ġ�plainlen���ĳ��ȡ�rand��ѡ�����
�����cipher����
���أ�0�ɹ� !0ʧ��
*/
int SM2_encrypt(unsigned char px[], unsigned char py[], unsigned char plain[], int plainlen, unsigned char rand[], unsigned char cipher[]);

/*
���ܣ�SM2�㷨˽Կ����
���룺privkey˽Կd��cipher���ġ�cipherlen���ĳ���
�����plain����
���أ�0�ɹ� !0ʧ��
*/
int SM2_decrypt(unsigned char privkey[], unsigned char cipher[], int cipherlen, unsigned char plain[]);

/*
���ܣ�SM3�㷨�����ݽ���HASH����
���룺msg��Ϣ��mlen��Ϣ����
�����hashժҪֵ
���أ���
*/
void SM3(unsigned char msg[], int mlen, unsigned char hash[]);

/*
���ܣ�SM4�㷨����
���룺in���ġ�inlen���ĳ��ȡ�key��Կ
�����out����
���أ�0�ɹ� !0ʧ��
*/
int SM4_encrypt(const unsigned char in[], int inlen, unsigned char out[], const unsigned char key[]);

/*
���ܣ�SM4�㷨����
���룺in���ġ�inlen���ĳ��ȡ�key��Կ
�����out����
���أ�0�ɹ� !0ʧ��
*/
int SM4_decrypt(const unsigned char in[], int inlen, unsigned char out[], const unsigned char key[]);


#ifdef __cplusplus
}
#endif


#endif
