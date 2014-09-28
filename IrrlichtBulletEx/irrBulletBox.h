#pragma once

#include <wtypes.h>
#include "irrlicht.h"

  namespace irr
{
namespace scene
{
  class CBulletAnimatorManager;
  class CBulletObjectAnimator;
  class CBulletWorldAnimator;
};
};


  class irrBulletBox
	:public irr::IEventReceiver
{
public:
	irrBulletBox(void);
	~irrBulletBox(void);

	irr::scene::CBulletAnimatorManager* m_pBulletPhysicsFactory;
	irr::scene::ISceneNode*  m_pWorldNode;
	irr::scene::CBulletWorldAnimator* m_pWorldAnimator;

	int Init(LPCWSTR lpclassName);
	void Apply(float fElapsedTime);
	void Render(float fElapsedTime);
	void Run();

	
	bool OnEvent(const irr::SEvent& event);

	void Close()
	{
		m_pDevice->closeDevice();
		m_pDevice->drop();			
	}

private:
	irr::IrrlichtDevice *m_pDevice;
};
