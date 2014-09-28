#include "StdAfx.h"
#include "irrBulletBox.h"

#include "CFollowAnimator.h"
#include "CBulletAnimatorManager.h"
#include "CBulletWorldAnimator.h"


irrBulletBox::irrBulletBox(void)
{
}

irrBulletBox::~irrBulletBox(void)
{
}

bool irrBulletBox::OnEvent(const irr::SEvent& event)
{
	
	
	if(event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		if(event.KeyInput.PressedDown)
		{
			irr::scene::ISceneManager *pSmgr = m_pDevice->getSceneManager();
			irr::video::IVideoDriver *pVideo = m_pDevice->getVideoDriver();

			{
				irr::scene::CBulletObjectAnimatorGeometry geom;
				irr::scene::CBulletObjectAnimatorParams physicsParams;

				irr::scene::ISceneNode *pNode = pSmgr->addCubeSceneNode(5.f);
				pNode->setPosition(irr::core::vector3df(3,20,0));
				pNode->setMaterialTexture(0,pVideo->getTexture("./res/irr_exam/t351sml.jpg"));

				geom.type = irr::scene::CBPAGT_BOX;
				geom.box.X = 2.5f;//.sphere.radius = radius;
				geom.box.Y = 2.5f;
				geom.box.Z = 2.5f;

				physicsParams.mass = .5f;
				physicsParams.gravity = core::vector3df(0, 0, 0);
				physicsParams.friction = 0.6f;

				irr::scene::CBulletObjectAnimator *pAnim = m_pBulletPhysicsFactory->createBulletObjectAnimator(
					pSmgr,
					pNode,
					m_pWorldAnimator->getID(),
					&geom,
					&physicsParams
					);
				pNode->addAnimator(pAnim);	
				pAnim->drop();
			}

			
		}
	}
	return false;
}

int irrBulletBox::Init(LPCWSTR lpclassName)
{
	irr::IrrlichtDevice *pDevice = irr::createDevice(					
			irr::video::EDT_DIRECT3D9,
			irr::core::dimension2d<u32>(640, 480), 32,
			false, false, true,
			this
			);
	m_pDevice = pDevice;		

	irr::scene::ISceneManager *pSmgr = m_pDevice->getSceneManager();
	irr::video::IVideoDriver *pVideo = m_pDevice->getVideoDriver();

	{
		irr::scene::ICameraSceneNode *pCam = pSmgr->addCameraSceneNodeMaya();
		pCam->setPosition(irr::core::vector3df(0,0,-50));
	}

	// Initialize bullet physics
	m_pBulletPhysicsFactory = new scene::CBulletAnimatorManager(pSmgr);

	m_pWorldNode = pSmgr->addEmptySceneNode();
	m_pWorldNode->setName("WorldNode");
	{
		irr::scene::CBulletWorldAnimatorParams worldParams;
		

		//루트애니매이터
		//irr::scene::CBulletWorldAnimator* bulletWorldAnimator = 
		m_pWorldAnimator =
			m_pBulletPhysicsFactory->createBulletWorldAnimator(
			pSmgr,
			m_pWorldNode,
			&worldParams
			);
		m_pWorldAnimator->setGravity(irr::core::vector3df(0,-5,0));
		m_pWorldNode->addAnimator(m_pWorldAnimator);
		//bulletWorldAnimator->drop();	

		

		//라이트 추가
		pSmgr->addLightSceneNode(0,irr::core::vector3df(20,20,0));
		pSmgr->addLightSceneNode(0,irr::core::vector3df(20,-20,0));

		{
			irr::scene::CBulletObjectAnimatorGeometry geom;
			irr::scene::CBulletObjectAnimatorParams physicsParams;

			irr::scene::ISceneNode *pNode = pSmgr->addCubeSceneNode(5.f);
			pNode->setPosition(irr::core::vector3df(3,20,0));
			pNode->setMaterialTexture(0,pVideo->getTexture("./res/irr_exam/t351sml.jpg"));

			geom.type = irr::scene::CBPAGT_BOX;
			geom.box.X = 2.5f;//.sphere.radius = radius;
			geom.box.Y = 2.5f;
			geom.box.Z = 2.5f;

			physicsParams.mass = .5f;
			physicsParams.gravity = core::vector3df(0, 0, 0);
			physicsParams.friction = 0.6f;

			irr::scene::CBulletObjectAnimator *pAnim = m_pBulletPhysicsFactory->createBulletObjectAnimator(
				pSmgr,
				pNode,
				m_pWorldAnimator->getID(),
				&geom,
				&physicsParams
				);
			pNode->addAnimator(pAnim);	
			pAnim->drop();
		}

		{
			irr::scene::CBulletObjectAnimatorGeometry geom;
			irr::scene::CBulletObjectAnimatorParams physicsParams;

			irr::scene::ISceneNode *pNode = pSmgr->addCubeSceneNode(5.f);
			//pNode->setPosition(irr::core::vector3df(0,20,0));
			pNode->setMaterialTexture(0,pVideo->getTexture("./res/irr_exam/t351sml.jpg"));

			geom.type = irr::scene::CBPAGT_BOX;
			geom.box.X = 2.5f;//.sphere.radius = radius;
			geom.box.Y = 2.5f;
			geom.box.Z = 2.5f;

			physicsParams.mass = 0.0f;
			physicsParams.gravity = core::vector3df(0, 0, 0);
			physicsParams.friction = 0.6f;

			irr::scene::CBulletObjectAnimator *pAnim = m_pBulletPhysicsFactory->createBulletObjectAnimator(
				pSmgr,
				pNode,
				m_pWorldAnimator->getID(),
				&geom,
				&physicsParams
				);
			pNode->addAnimator(pAnim);	

			pAnim->drop();
		}

		{			
			irr::scene::CBulletObjectAnimatorGeometry geom;
			irr::scene::CBulletObjectAnimatorParams physicsParams;

			irr::scene::ISceneNode* pNode = 0;
			irr::scene::IAnimatedMesh* pMesh;			

			pMesh = pSmgr->addHillPlaneMesh("myHill",
				irr::core::dimension2d<irr::f32>(40,40),
				irr::core::dimension2d<irr::u32>(10,10), 
				0,0,
				irr::core::dimension2d<irr::f32>(0,0),
				irr::core::dimension2d<irr::f32>(10,10));

			pNode = pSmgr->addAnimatedMeshSceneNode(pMesh);
			pNode->setPosition(irr::core::vector3df(0,-20,0));
			pNode->setMaterialTexture(0, pVideo->getTexture("./res/irr_exam/wall.jpg"));	

			// add level static mesh
			geom.type = scene::CBPAGT_CONCAVE_MESH;
			//geom.type = scene::CBPAGT_CONCAVE_GIMPACT_MESH;
			geom.mesh.irrMesh = pMesh->getMesh(0);
			geom.mesh.irrMesh = NULL;
			geom.meshFile = "myHill";//maps/20kdm2.bsp";

			physicsParams.mass = 0.0f;

			scene::CBulletObjectAnimator* levelAnim = 
				m_pBulletPhysicsFactory->createBulletObjectAnimator(
				pSmgr,
				pNode,
				m_pWorldAnimator->getID(),
				&geom,
				&physicsParams
				);
			pNode->addAnimator(levelAnim);
			levelAnim->drop();
		}
	}

	return 0;
}

void irrBulletBox::Apply(float fElapsedTime)
{
	//애니매이터나 캐릭터 애니메이션을 시키려면 tick을 호출해야함
	m_pDevice->getTimer()->tick();
	m_pBulletPhysicsFactory->OnUpdate(m_pDevice->getTimer()->getTime());
}

void irrBulletBox::Render(float fElapsedTime)
{
	irr::video::IVideoDriver *pVideo = m_pDevice->getVideoDriver();

	if(pVideo->beginScene(true, true, irr::video::SColor(255,100,101,140)) )
	{	
		irr::scene::ISceneManager *pSmgr = m_pDevice->getSceneManager();

		pSmgr->drawAll();		
		
		pVideo->endScene();
	}	

}

void irrBulletBox::Run()
{
	while (m_pDevice->run())
	{
		Apply(0);
		Render(0);
	}

}
