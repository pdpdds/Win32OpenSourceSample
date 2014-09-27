// tplEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "tpl.h"
#include <malloc.h>

#pragma comment(lib, "tpl.lib")

void Save(char* szFileName)
{
	tpl_node *tn;
	int id = 0;
	char *name, *names[] = { "joe", "bob", "cary" };

	tn = tpl_map("A(is)", &id, &name);

	for (name = names[0]; id < 3; name = names[++id]) {
		tpl_pack(tn, 1);
	}

	tpl_dump(tn, TPL_FILE, szFileName);
	tpl_free(tn);
}

void Load(char* szFileName)
{
	tpl_node *tn;
	int id;
	char *name;

	tn = tpl_map("A(is)", &id, &name);
	tpl_load(tn, TPL_FILE, szFileName);

	while (tpl_unpack(tn, 1) > 0) {
		printf("id %d, user %s\n", id, name);
		free(name);
	}
	tpl_free(tn);
}

int main(int argc, char *argv[]) 
{
	Save("users.tpl");
	Load("users.tpl");
}