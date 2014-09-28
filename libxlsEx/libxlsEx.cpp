// libxlsEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#if defined (__cplusplus)
extern "C" {
#endif


#include "libxls/xls.h"

#if defined (__cplusplus)
}
#endif

#pragma comment(lib, "libxls.lib")

int main(int argc, char *argv[])
{
	xlsWorkBook* pWB;
	xlsWorkSheet* pWS;
	st_row_data* row;
	WORD t, tt;
	
//액셀 문서를 연다.
	pWB = xls_open("test.xls", "UTF-8");	

	if (pWB != NULL)
	{		
//워크 시트 갯수만큼 루프를 돈다.
		for (int i = 0; i<pWB->sheets.count; i++) 
		{
			printf("Sheet[%i] (%s) pos=%i\n", i, pWB->sheets.sheet[i].name, pWB->sheets.sheet[i].filepos);

//워크 시트를 얻어온 뒤
//데이터를 파싱한다.
			pWS = xls_getWorkSheet(pWB, i);

			xls_parseWorkSheet(pWS);

//워크 시트의 내용을 출력한다.
			printf("Count of rows: %i\n", pWS->rows.lastrow + 1);
			printf("Max col: %i\n", pWS->rows.lastcol);

			for (t = 0; t <= pWS->rows.lastrow; t++)
			{
				row = (st_row_data*)&pWS->rows.row[t];
				
				//xls_showROW(row);

				for (tt = 0; tt <= pWS->rows.lastcol; tt++)
				{
					st_cell_data cell = row->cells.cell[tt];
					printf("cell=%s\n", cell.str);

					//xls_showCell((st_cell_data *)&row->cells.cell[tt]);
				}
			}

		}
		//xls_showBookInfo(pWB);
	}
	
	return 0;
}