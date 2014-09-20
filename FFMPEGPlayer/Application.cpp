#include "Application.h"
#include "Window.h"

#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreColourValue.h>

using namespace Ogre;

Application::Application()
{
}

//------------------------------------------------------------------------------//

Application::~Application()
{
	
}

//------------------------------------------------------------------------------//

void Application::createScene()
{
	mSceneMgr = Root::getSingleton().createSceneManager(ST_GENERIC);
	
	mCamera = mSceneMgr->createCamera("Camera");

  mRectangle = new Rectangle2D(true);
  mRectangle->setCorners(-1.0, 1.0, 1.0, -1.0);
  Ogre::AxisAlignedBox aabInf;
  aabInf.setInfinite();
  mRectangle->setBoundingBox(aabInf);

  Ogre::SceneNode* node = mSceneMgr->getRootSceneNode ()->createChildSceneNode ();
  node->attachObject (mRectangle);

  Ogre::MaterialPtr videoMaterial = Ogre::MaterialManager::getSingleton ().create ("VideoMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  videoMaterial->getTechnique (0)->getPass(0)->createTextureUnitState ();

  mRectangle->setMaterial ("VideoMaterial");

	onRenderWindowRecreated();
}

//------------------------------------------------------------------------------//

void Application::onRenderWindowRecreated()
{
	mViewport = mWindow->mRenderWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(ColourValue(0.2, 0.2, 0.2, 1.0));
}

//------------------------------------------------------------------------------//

void Application::update (float dt)
{
}

bool Application::render ()
{
  Ogre::MaterialPtr videoMaterial = Ogre::MaterialManager::getSingleton ().getByName("VideoMaterial");

  if (!Ogre::TextureManager::getSingleton ().getByName ("VideoTexture").isNull ())
    videoMaterial->getTechnique(0)->getPass(0)->getTextureUnitState (0)->setTextureName ("VideoTexture");


    Ogre::WindowEventUtilities::messagePump();
  Ogre::Root::getSingleton ().renderOneFrame ();

  return !mShutdown;
}
