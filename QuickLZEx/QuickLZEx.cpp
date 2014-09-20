// QuickLZEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "quicklz.h"
#include <stdio.h>
#include <stdlib.h>

void FileCompress(char* szSrcFile, char* szDestFile);
void FileDecompress(char* szSrcFile, char* szDestFile);

int main(int argc, char* argv[])
{
	if (argc != 4)
		return 0;

	if (strcmp(argv[1], "-c") == 0)
		FileCompress(argv[2], argv[3]);
	else if (strcmp(argv[1], "-u") == 0)
		FileDecompress(argv[2], argv[3]);

	
	return 0;
}

void FileCompress(char* szSrcFile, char* szDestFile)
{
	FILE *ifile, *ofile;
	char *src, *dst;
	qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));
	size_t len, len2;
	ifile = fopen(szSrcFile, "rb");
	ofile = fopen(szDestFile, "wb");

	// allocate source buffer and read file
	fseek(ifile, 0, SEEK_END);
	len = ftell(ifile);
	fseek(ifile, 0, SEEK_SET);
	src = (char*)malloc(len);
	fread(src, 1, len, ifile);

	// allocate "uncompressed size" + 400 for the destination buffer
	dst = (char*)malloc(len + 400);

	// compress and write result
	len2 = qlz_compress(src, dst, len, state_compress);
	fwrite(dst, len2, 1, ofile);
	fclose(ifile);
	fclose(ofile);
}

void FileDecompress(char* szSrcFile, char* szDestFile)
{
	FILE *ifile, *ofile;
	char *src, *dst;
	qlz_state_decompress *state_decompress = (qlz_state_decompress *)malloc(sizeof(qlz_state_decompress));
	unsigned int len;

	ifile = fopen(szSrcFile, "rb");
	ofile = fopen(szDestFile, "wb");

	// allocate source buffer
	fseek(ifile, 0, SEEK_END);
	len = ftell(ifile);
	fseek(ifile, 0, SEEK_SET);
	src = (char*)malloc(len);

	// read file and allocate destination buffer
	fread(src, 1, len, ifile);
	len = qlz_size_decompressed(src);
	dst = (char*)malloc(len);

	// decompress and write result
	len = qlz_decompress(src, dst, state_decompress);
	fwrite(dst, len, 1, ofile);
	fclose(ifile);
	fclose(ofile);
}