#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>

namespace Ogre { class Root; class RenderWindow; }

class Window; class InputManager;

class BaseApplication : public Ogre::WindowEventListener, public Ogre::FrameListener
{
public:
	BaseApplication();
	~BaseApplication();
	
	void run();
	
protected:
	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);
	
	void recreateWindow();
	
	size_t mWidth;
	size_t mHeight;
	
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual void update (float dt) = 0;

	bool mShutdown;

	Ogre::Root* mRoot;
	
	//InputManager* mInputManager;

	Window* mWindow;
	
	virtual void createScene() = 0;
	virtual void onRenderWindowRecreated() = 0;
};
