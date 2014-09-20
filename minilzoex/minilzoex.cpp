// lzoex.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "minilzo.h"
#include <stdlib.h>
#pragma comment(lib, "minilzo.lib")

/* Work-memory needed for compression. Allocate memory in units
* of 'lzo_align_t' (instead of 'char') to make sure it is properly aligned.
*/

#define HEAP_ALLOC(var,size) \
	lzo_align_t __LZO_MMODEL var[((size)+(sizeof(lzo_align_t)-1)) / sizeof(lzo_align_t)]

static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);

int _tmain(int argc, _TCHAR* argv[])
{
	lzo_bytep in;
	lzo_bytep out;
	lzo_voidp wrkmem;
	lzo_uint in_len;
	lzo_uint out_len;
	lzo_uint new_len;

	//lzo 초기화
	if (lzo_init() != LZO_E_OK)
	{
		printf("lzo_init() failed\n");
		return 0;
	}

	//파일 로딩
	FILE* fp = 0;
	errno_t err = fopen_s(&fp, "LICENSE", "rb");
	if (err != 0)
		return 0;

	fseek(fp, 0L, SEEK_END);
	in_len = ftell(fp);

	fseek(fp, 0L, SEEK_SET);

	//메모리 할당
	in = (lzo_bytep)malloc(in_len);
	out_len = in_len + in_len / 16 + 64 + 3;
	out = (lzo_bytep)malloc(out_len);
	wrkmem = (lzo_voidp)malloc(LZO1X_1_MEM_COMPRESS);
	if (in == NULL || out == NULL || wrkmem == NULL)
	{
		printf("out of memory\n");
		return 0;
	}

	//데이터를 읽어 들인다.
	fread(in, in_len, 1, fp);
	fclose(fp);

	/*
	* Step 3: compress from 'in' to 'out' with LZO1X-1
	*/
	int r = lzo1x_1_compress(in, in_len, out, &out_len, wrkmem);
	if (r == LZO_E_OK)
		printf("compressed %lu bytes into %lu bytes\n",
		(unsigned long)in_len, (unsigned long)out_len);
	else
	{
		/* this should NEVER happen */
		printf("internal error - compression failed: %d\n", r);
		return 2;
	}
	/* check for an incompressible block */
	if (out_len >= in_len)
	{
		printf("This block contains incompressible data.\n");
		return 0;
	}

	/*
	* Step 4: decompress again, now going from 'out' to 'in'
	*/
	new_len = in_len;
	r = lzo1x_decompress(out, out_len, in, &new_len, NULL);
	if (r == LZO_E_OK && new_len == in_len)
		printf("decompressed %lu bytes back into %lu bytes\n",
		(unsigned long)out_len, (unsigned long)in_len);
	else
	{
		/* this should NEVER happen */
		printf("internal error - decompression failed: %d\n", r);
		return 1;
	}

	free(wrkmem);
	free(out);
	free(in);
	printf("Simple compression test passed.\n");

	return 0;
}

