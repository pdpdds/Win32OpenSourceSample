#pragma once
#include "Bass.h"

#define SPECWIDTH 320	// display width
#define SPECHEIGHT 127	// height (changing requires palette adjustments too)

#define ECHBUFLEN 1200	// buffer length
#define FLABUFLEN 350	// buffer length

#define EFFECT_FLANGER       0		
#define EFFECT_ROTATE        1                        					
#define EFFECT_ECHO          2               
#define UNEFFECT_FLANGER	 3	
#define UNEFFECT_ROTATE      4                     					
#define UNEFFECT_ECHO        5 

#pragma comment(lib, "bass.lib")

class CBassSoundManager
{
public:
	CBassSoundManager(void);
	~CBassSoundManager(void);
	BOOL Initialize(void);
	bool Play(BOOL bLoop = FALSE);
	
private:
// BASS!!
	
	DWORD m_volume;
	DWORD floatable; // floating-point channel support?
	DWORD chan;	// the channel... HMUSIC or HSTREAM	
	int specmode;
	int specpos; // spectrum mode (and marker pos for 2nd mode)	
	BYTE specbuf[SPECWIDTH*SPECHEIGHT];
	BYTE specbuf2[SPECWIDTH];
	float specbuf2timeset[SPECWIDTH*2];
		
public:
	void SetVolumeDown(void);
	void SetVolumeUp(void);
	void SetEffect(int nEffectType);	


	
	BOOL FileLoad(char *);
	void Delete(void);
	void GetVolume(void);
	void GetCurrentTimea(void);

	static void CALLBACK Echo(HDSP handle, DWORD channel, void *buffer, DWORD length, DWORD user);
	static void CALLBACK Rotate(HDSP handle, DWORD channel, void *buffer, DWORD length, DWORD user);
	static void CALLBACK Flange(HDSP handle, DWORD channel, void *buffer, DWORD length, DWORD user);
};