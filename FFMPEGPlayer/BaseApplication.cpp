#include "BaseApplication.h"
#include "PathManager.h"
#include "Window.h"
#include "InputManager.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OIS/OIS.h>

using namespace Ogre;

//------------------------------------------------------------------------------//

BaseApplication::BaseApplication() :
  mWindow(0), mWidth(1024), mHeight(768), mShutdown(false)
{

}

//------------------------------------------------------------------------------//

BaseApplication::~BaseApplication()
{

    // Shutdown
//	delete mInputManager;
    delete mWindow;
    mRoot->removeFrameListener(this);
    delete mRoot;
}

//------------------------------------------------------------------------------//

void BaseApplication::run()
{	
	// Construct Ogre::Root
	// We handle our stuff manually, so don't want to use any of the files
	mRoot = new Root(
		/* plugins.cfg file*/	"",
		/* config file */ 		"", 
		/* log file */ 			""
	);

    PathManager::init ();
	
	// Set render system
	//!todo do not hardcode render system
	
#ifdef _DEBUG 
	mRoot->loadPlugin(PathManager::ogre_plugin_dir + "/RenderSystem_GL");
	RenderSystem* rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
#else
	mRoot->loadPlugin(PathManager::ogre_plugin_dir + "/RenderSystem_Direct3D9");
	RenderSystem* rs = mRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem");
#endif
	mRoot->setRenderSystem(rs);
	
	// Fire up Ogre::Root
	mRoot->initialise(false);

	// Create a hidden background window to keep resources
  NameValuePairList params;
	params.insert(std::make_pair("hidden", "true"));
	RenderWindow *wnd = mRoot->createRenderWindow("InactiveHidden", 1, 1, false, &params);
	wnd->setActive(false);

  Ogre::ResourceGroupManager::getSingleton ().addResourceLocation ("./", "FileSystem");
  Ogre::ResourceGroupManager::getSingleton ().initialiseAllResourceGroups ();

	// Create input system
	//mInputManager = new InputManager();

	// Create the application window
	mWindow = new Window();
	mWindow->mListener = static_cast<WindowEventListener*>(this);
	//mWindow->mInputManager = mInputManager;
    mWindow->mWidth = 800;
    mWindow->mHeight = 600;
    mWindow->mFullscreen = false;
    mWindow->create();
		
	createScene();
	mRoot->addFrameListener(this);
}

//------------------------------------------------------------------------------//

void BaseApplication::windowResized(RenderWindow* rw)
{
	
}

//------------------------------------------------------------------------------//

void BaseApplication::windowClosed(RenderWindow* rw)
{
	mShutdown = true;
}

//------------------------------------------------------------------------------//

void BaseApplication::recreateWindow()
{
	mWindow->recreate();
	onRenderWindowRecreated();
}

//------------------------------------------------------------------------------//

bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	
	InputEvent* ev = 0;
	
	//mInputManager->process();
	
	while (true)
	{
		//ev = mInputManager->pollEvents();
		
		if (ev == 0) break;
				
		if (ev->eventType == ET_Keyboard)
		{
			KeyEvent* kev = static_cast<KeyEvent*>(ev);
			if (kev->keyCode == OIS::KC_ESCAPE)
				mShutdown = true;
		}

		delete ev;
	}

    update(evt.timeSinceLastFrame);
	
	return !mShutdown;
}
