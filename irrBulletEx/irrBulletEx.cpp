// irrBulletEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <irrlicht.h>
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "LinearMath_vs2010_debug.lib")
#pragma comment(lib, "BulletDynamics_vs2010_debug.lib")
#pragma comment(lib, "BulletCollision_vs2010_debug.lib")
#pragma comment(lib, "BulletSoftBody_vs2010_debug.lib")
#pragma comment(lib, "Bullet3Dynamics_vs2010_debug.lib")
#else
#pragma comment(lib, "LinearMath_vs2010.lib")
#pragma comment(lib, "BulletDynamics_vs2010.lib")
#pragma comment(lib, "BulletCollision_vs2010.lib")
#pragma comment(lib, "BulletSoftBody_vs2010.lib")
#endif

#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "Irrbullet.lib")

#include "raycasttankexample.h"
#include "liquidbodyexample.h"
#include "characterexample.h"
#include "CollisionExample.h"
#include "helloworldexample.h"
#include "LoadSceneExample.h"

using namespace std;

int main()
{
	//CLiquidbodyExample example;
	//CCharacterExample  example;
	//CCollisionExample example;
	CHelloWorldExample example;
	//CLoadSceneExample example;

	example.runExample();

	return 0;
}