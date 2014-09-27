// libtomEx4.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <tomcrypt.h>

#pragma comment(lib, "tomcrypt.lib")

bool executeRC4(unsigned char* pBuffer, int nBufferLen)
{
	prng_state prng; //의사 랜덤 넘버 제너레이터 

	int err;
	if ((err = rc4_start(&prng)) != CRYPT_OK) //RC4 초기화 
		return 0;
	//키 "Win32OpenSource"를 더해서 엔트로피를 증가시킴 
	if ((err = rc4_add_entropy((const unsigned char*)"Win32OpenSource", 15, &prng)) != CRYPT_OK)
		return 0;

	//RC4 준비 상태.. PRNG Init 
	if ((err = rc4_ready(&prng)) != CRYPT_OK)
		return 0;

	if (rc4_read(pBuffer, nBufferLen, &prng) != nBufferLen)
		return 0;

	//암호화 결과 pBuffer에 저장이 됨 

	rc4_done(&prng);

	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{

	unsigned char buf[32] = "Hello World";
	int nBufferLen = 11;
	executeRC4(buf, nBufferLen);
	executeRC4(buf, nBufferLen);

	return 0;
}

int SymmetricTest()
{
	unsigned char key[16], IV[16], buffer[512] = "you are blooding";
	symmetric_CTR ctr;
	int x, err;
	/* register twofish first */
	if (register_cipher(&twofish_desc) == -1) {
		printf("Error registering cipher.\n");
		return -1;
	}
	/* somehow fill out key and IV */
	/* start up CTR mode */
	if ((err = ctr_start(
		find_cipher("twofish"), /* index of desired cipher */
		IV, /* the initial vector */
		key, /* the secret key */
		16, /* length of secret key (16 bytes) */
		0, /* 0 == default # of rounds */
		CTR_COUNTER_LITTLE_ENDIAN, /* Little endian counter */
		&ctr) /* where to store the CTR state */
		) != CRYPT_OK) {
		printf("ctr_start error: %s\n", error_to_string(err));
		return -1;
	}
	/* somehow fill buffer than encrypt it */
	if ((err = ctr_encrypt(buffer, /* plaintext */
		buffer, /* ciphertext */
		sizeof(buffer), /* length of plaintext pt */
		&ctr) /* CTR state */
		) != CRYPT_OK) {
		printf("ctr_encrypt error: %s\n", error_to_string(err));
		return -1;
	}
	/* make use of ciphertext... */
	/* now we want to decrypt so let’s use ctr_setiv */
	if ((err = ctr_setiv(IV, /* the initial IV we gave to ctr_start */
		16, /* the IV is 16 bytes long */
		&ctr) /* the ctr state we wish to modify */
		) != CRYPT_OK) {
		printf("ctr_setiv error: %s\n", error_to_string(err));
		return -1;
	}
	if ((err = ctr_decrypt(buffer, /* ciphertext */
		buffer, /* plaintext */
		sizeof(buffer), /* length of plaintext */
		&ctr) /* CTR state */
		) != CRYPT_OK) {
		printf("ctr_decrypt error: %s\n", error_to_string(err));
		return -1;
	}
	/* terminate the stream */
	if ((err = ctr_done(&ctr)) != CRYPT_OK) {
		printf("ctr_done error: %s\n", error_to_string(err));
		return -1;
	}
	/* clear up and return */
	zeromem(key, sizeof(key));
	zeromem(&ctr, sizeof(ctr));

	return 0;
}

int HashTest()
{
	unsigned char buffer[100], hash[MAXBLOCKSIZE];
	int idx, x;
	hash_state md;
	/* register hashes .... */
	if (register_hash(&md5_desc) == -1) {
		printf("Error registering MD5.\n");
		return -1;
	}
	/* register other hashes ... */
	/* prompt for name and strip newline */
	printf("Enter hash name: \n");
	fgets((char*)buffer, sizeof(buffer), stdin);
	buffer[strlen((char*)buffer) - 1] = 0;
	/* get hash index */
	idx = find_hash((const char*)buffer);
	if (idx == -1) {
		printf("Invalid hash name!\n");
		return -1;
	}
	/* hash input until blank line */
	hash_descriptor[idx].init(&md);
	//while (fgets((char*)buffer, sizeof(buffer), stdin) != NULL)

	fgets((char*)buffer, sizeof(buffer), stdin);
	hash_descriptor[idx].process(&md, buffer, strlen((char*)buffer));
	hash_descriptor[idx].done(&md, hash);
	/* dump to screen */
	for (x = 0; x < hash_descriptor[idx].hashsize; x++)
		printf("%02x ", hash[x]);
	printf("\n");
	return 0;
}

int HashMemory(void)
{
	int idx, err;
	unsigned long len;
	unsigned char out[MAXBLOCKSIZE];
	/* register the hash */
	if (register_hash(&md5_desc) == -1) {
		printf("Error registering MD5.\n");
		return -1;
	}
	/* get the index of the hash */
	idx = find_hash("md5");
	/* call the hash */
	len = sizeof(out);
	if ((err =
		hash_memory(idx, (const unsigned char*)"hello world", 11, out, &len)) != CRYPT_OK) {
		printf("Error hashing data: %s\n", error_to_string(err));
		return -1;
	}
	return 0;
}

int hmac()
{
	int idx, err;
	hmac_state hmac;
	unsigned char key[16], dst[MAXBLOCKSIZE];
	unsigned long dstlen;
	/* register SHA-1 */
	if (register_hash(&sha1_desc) == -1) {
		printf("Error registering SHA1\n");
		return -1;
	}
	/* get index of SHA1 in hash descriptor table */
	idx = find_hash("sha1");
	/* we would make up our symmetric key in "key[]" here */
	/* start the HMAC */
	if ((err = hmac_init(&hmac, idx, key, 16)) != CRYPT_OK) {
		printf("Error setting up hmac: %s\n", error_to_string(err));
		return -1;
	}
	/* process a few octets */
	if (err = hmac_process(&hmac, (const unsigned char*)"hello", 5) != CRYPT_OK) {
		printf("Error processing hmac: %s\n", error_to_string(err));
		return -1;
	}
	/* get result (presumably to use it somehow...) */
	dstlen = sizeof(dst);
	if ((err = hmac_done(&hmac, dst, &dstlen)) != CRYPT_OK) {
		printf("Error finishing hmac: %s\n", error_to_string(err));
		return -1;
	}
	printf("The hmac is %lu bytes long\n", dstlen);
	/* return */
	return 0;
}

int omac(void)
{
	int idx, err;
	omac_state omac;
	unsigned char key[16], dst[MAXBLOCKSIZE];
	unsigned long dstlen;
	/* register Rijndael */
	if (register_cipher(&rijndael_desc) == -1) {
		printf("Error registering Rijndael\n");
		return -1;
	}
	/* get index of Rijndael in cipher descriptor table */
	idx = find_cipher("rijndael");
	/* we would make up our symmetric key in "key[]" here */
	/* start the OMAC */
	if ((err = omac_init(&omac, idx, key, 16)) != CRYPT_OK) {
		printf("Error setting up omac: %s\n", error_to_string(err));
		return -1;
	}
	/* process a few octets */
	if (err = omac_process(&omac, (unsigned char*)"hello", 5) != CRYPT_OK) {
		printf("Error processing omac: %s\n", error_to_string(err));
		return -1;
	}
	/* get result (presumably to use it somehow...) */
	dstlen = sizeof(dst);
	if ((err = omac_done(&omac, dst, &dstlen)) != CRYPT_OK) {
		printf("Error finishing omac: %s\n", error_to_string(err));
		return -1;
	}
	printf("The omac is %lu bytes long\n", dstlen);
	/* return */
	return 0;
}

int prnggenerator(void)
{
	prng_state prng;
	unsigned char buf[10];
	int err;
	/* start it */
	if ((err = yarrow_start(&prng)) != CRYPT_OK) {
		printf("Start error: %s\n", error_to_string(err));
	}
	/* add entropy */
	if ((err = yarrow_add_entropy((const unsigned char*)"hello world", 11, &prng))
		!= CRYPT_OK) {
		printf("Add_entropy error: %s\n", error_to_string(err));
	}
	/* ready and read */
	if ((err = yarrow_ready(&prng)) != CRYPT_OK) {
		printf("Ready error: %s\n", error_to_string(err));
	}
	printf("Read %lu bytes from yarrow\n",
		yarrow_read(buf, sizeof(buf), &prng));
	return 0;
}

int aecc_key(void)
{
	ecc_key mykey;
	prng_state prng;
	int err;
	/* register yarrow */
	if (register_prng(&yarrow_desc) == -1) {
		printf("Error registering Yarrow\n");
		return -1;
	}
	/* setup the PRNG */
	if ((err = rng_make_prng(128, find_prng("yarrow"), &prng, NULL))
		!= CRYPT_OK) {
		printf("Error setting up PRNG, %s\n", error_to_string(err));
		return -1;
	}
	/* make a 192-bit ECC key */
	if ((err = ecc_make_key(&prng, find_prng("yarrow"), 24, &mykey))
		!= CRYPT_OK) {
		printf("Error making key: %s\n", error_to_string(err));
		return -1;
	}
	return 0;
}