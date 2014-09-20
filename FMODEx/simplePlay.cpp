// simplePlay.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "fmod.hpp"
#include <Windows.h>
#include <stdlib.h>

#pragma comment(lib, "fmod_vc.lib")

void ERRCHECK(FMOD_RESULT result)
{
	if (FMOD_OK != result)
		exit(0);
}

int _tmain(int argc, _TCHAR* argv[])
{
	FMOD::System     *system;
	FMOD::Sound      *sound;
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;

	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->getVersion(&version);
	ERRCHECK(result);
	
	if (version < FMOD_VERSION)
	{
		return -1;
	}

	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ERRCHECK(result);

	result = system->createSound("5.mp3", FMOD_DEFAULT, 0, &sound);
	ERRCHECK(result);

	result = sound->setMode(FMOD_LOOP_OFF);   
	ERRCHECK(result);

	result = system->playSound(sound, 0, false, &channel);
	ERRCHECK(result);

	bool          playing = false;
	do
	{
		result = system->update();
		ERRCHECK(result);

		result = channel->isPlaying(&playing);

		Sleep(50);
	} while (playing == true);

	result = sound->release();
	ERRCHECK(result);

	ERRCHECK(result);
	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);

	return 0;
}

