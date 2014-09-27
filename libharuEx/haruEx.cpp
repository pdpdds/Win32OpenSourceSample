// haruexample.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "hpdf.h"

#ifdef _DEBUG
#pragma comment(lib, "libhpdfd.lib")
#else
#pragma comment(lib, "libhpdf.lib")
#endif

jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler(HPDF_STATUS   error_no,
HPDF_STATUS   detail_no,
void         *user_data)
{
	printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
		(HPDF_UINT)detail_no);
	longjmp(env, 1);
}

const char *font_list[] = {
	"Courier",
	"Courier-Bold",
	"Courier-Oblique",
	"Courier-BoldOblique",
	"Helvetica",
	"Helvetica-Bold",
	"Helvetica-Oblique",
	"Helvetica-BoldOblique",
	"Times-Roman",
	"Times-Bold",
	"Times-Italic",
	"Times-BoldItalic",
	"Symbol",
	"ZapfDingbats",
	NULL
};

int main(int argc, char **argv)
{
	const char *page_title = "Font Demo";
	HPDF_Doc  pdf;
	char fname[256];
	HPDF_Page page;
	HPDF_Font def_font;
	HPDF_REAL tw;
	HPDF_REAL height;
	HPDF_REAL width;
	HPDF_UINT i;

	strcpy_s(fname, argv[0]);
	strcat_s(fname, ".pdf");

//PDF 객체를 생성한다.
	pdf = HPDF_New(error_handler, NULL);
	if (!pdf) {
		printf("error: cannot create PdfDoc object\n");
		return 1;
	}

	if (setjmp(env)) {
		HPDF_Free(pdf);
		return 1;
	}

//페이지를 추가한다.
	/* Add a new page object. */
	page = HPDF_AddPage(pdf);

//페이지의 높이와 너비를 구한다.
	height = HPDF_Page_GetHeight(page);
	width = HPDF_Page_GetWidth(page);

//선의 두께를 지정한다.	
	HPDF_Page_SetLineWidth(page, 1);
//사각형을 그린다.
	HPDF_Page_Rectangle(page, 50, 50, width - 100, height - 110);
	HPDF_Page_Stroke(page);

	/* Print the title of the page (with positioning center). */
	def_font = HPDF_GetFont(pdf, "Helvetica", NULL);
	HPDF_Page_SetFontAndSize(page, def_font, 24);

	tw = HPDF_Page_TextWidth(page, page_title);
	HPDF_Page_BeginText(page);
	HPDF_Page_TextOut(page, (width - tw) / 2, height - 50, page_title);
	HPDF_Page_EndText(page);

	/* output subtitle. */
	HPDF_Page_BeginText(page);
	HPDF_Page_SetFontAndSize(page, def_font, 16);
	HPDF_Page_TextOut(page, 60, height - 80, "<Standerd Type1 fonts samples>");
	HPDF_Page_EndText(page);

	HPDF_Page_BeginText(page);
	HPDF_Page_MoveTextPos(page, 60, height - 105);

	i = 0;
	while (font_list[i]) {
		const char* samp_text = "abcdefgABCDEFG12345!#$%&+-@?";
		HPDF_Font font = HPDF_GetFont(pdf, font_list[i], NULL);

		/* print a label of text */
		HPDF_Page_SetFontAndSize(page, def_font, 9);
		HPDF_Page_ShowText(page, font_list[i]);
		HPDF_Page_MoveTextPos(page, 0, -18);

		/* print a sample text. */
		HPDF_Page_SetFontAndSize(page, font, 20);
		HPDF_Page_ShowText(page, samp_text);
		HPDF_Page_MoveTextPos(page, 0, -20);

		i++;
	}

	HPDF_Page_EndText(page);

	HPDF_SaveToFile(pdf, fname);

	/* clean up */
	HPDF_Free(pdf);

	return 0;
}
