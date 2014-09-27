// lokkiEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>

#include "SmallObj.h"

struct Obj
{
	int i;
	float f;
};

int main()
{
	Loki::FixedAllocator fa(sizeof(Obj));
	Loki::SmallObjAllocator sa(3 * sizeof(Obj), sizeof(Obj));

	Obj * p1[4];
	Obj * p2[4];
	for (int k = 0; k<4; k++) {
		p1[k] = reinterpret_cast<Obj*>(fa.Allocate());
		p2[k] = (Obj*)sa.Allocate(sizeof(Obj));
		std::cout << p1[k] << " " << p2[k] << "\n";
	}
	for (int k = 0; k<4; k++) {
		fa.Deallocate(p1[k]);
		sa.Deallocate(p2[k], sizeof(Obj));
	}

	typedef Loki::SmallObject< Loki::SingleThreaded,
		3 * sizeof(Obj),
		sizeof(Obj) > sobj;
	for (int k = 0; k<4; k++) {
		p1[k] = (Obj*)sobj::operator new(sizeof(Obj));
		p2[k] = (Obj*)sobj::operator new(sizeof(Obj));
		std::cout << p1[k] << " " << p2[k] << "\n";
	}
	for (int k = 0; k<4; k++) {
		sobj::operator delete(p1[k], sizeof(Obj));
	}
	for (int k = 0; k<4; k++) {
		std::cout << p2[k] << "\n";
	}
	for (int k = 0; k<4; k++) {
		sobj::operator delete(p2[k], sizeof(Obj));
	}
}