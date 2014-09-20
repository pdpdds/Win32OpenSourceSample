// sparshHashEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <google/dense_hash_map>

using google::dense_hash_map;      // namespace where class lives by default

int main()
{
	dense_hash_map<const char*, int> months;

	months.set_empty_key(NULL);
	months["january"] = 31;
	months["february"] = 28;
	months["march"] = 31;
	months["april"] = 30;
	months["may"] = 31;
	months["june"] = 30;
	months["july"] = 31;
	months["august"] = 31;
	months["september"] = 30;
	months["october"] = 31;
	months["november"] = 30;
	months["december"] = 31;

	for (google::dense_hash_map<const char*, int>::const_iterator it = months.begin();
		it != months.end(); ++it)
		printf("months: %s -> %d\n", it->first, it->second);

	return 0;
}