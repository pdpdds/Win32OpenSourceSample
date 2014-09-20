// BerkeleyDBEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
// BerkeleyDBEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "db_cxx.h"
#include <assert.h>

#ifdef _DEBUG
#pragma comment(lib, "libdb61d.lib")
#else
#pragma comment(lib, "libdb61.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	Db db(NULL, 0); // 데이터베이스 오브젝트 초기화
	u_int32_t oFlags = DB_CREATE; // 데이터베이스가 존재하지 않으면 생성할 것

	//데이터베이스 오픈
	try {
		db.open(NULL, //트랜잭션 오브젝트에 대한 포인터
			"my_db.db", // 데이터베이스 파일 이름
			NULL,
			DB_BTREE, // 데이터베이스의 엔트리에 접근하는 방법
			oFlags, // 플래그
			0);
	}
	catch (DbException &e) {
	}
	catch (std::exception &e) {
	}

	float money = 122.45; //키
	char *description = "Grocery bill."; //데이터

	//데이터베이스에 쓰기
	{

		Dbt key(&money, sizeof(float));
		Dbt data(description, strlen(description) + 1);
		//키,데이터 쌍을 데이터베이스에 기록할 것. 단 키가 존재한다면 덮어쓰지 말 것
		int ret = db.put(NULL, &key, &data, DB_NOOVERWRITE);
		if (ret == DB_KEYEXIST) {
			db.err(ret, "Put failed because key %f already exists",
				money);
		}
	}

	//데이터베이스에서 읽기
	{
#define DESCRIPTION_SIZE 199
		float money;
		char szDescription[DESCRIPTION_SIZE + 1];
		// Database open omitted for clarity	
		money = 122.45;
		Dbt key, data;
		key.set_data(&money);
		key.set_size(sizeof(float));
		data.set_data(szDescription);
		data.set_ulen(DESCRIPTION_SIZE + 1);
		data.set_flags(DB_DBT_USERMEM);

		//데이터베이스에서 키 값이 122.45을 찾아서 그것의 데이터를 얻어온다.
		db.get(NULL, &key, &data, 0);

		//읽어온 데이터값과 최초에 쓴 데이터값은 일치해야 한다.
		assert(strcmp(description, szDescription) == 0);
	}

	//데이터베이스를 닫는다.
	try {
		db.close(0);
	}
	catch (DbException &e) {
		
	}
	catch (std::exception &e) {
	}

	return 0;
}

